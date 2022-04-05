#include <cassert>
#include "Parser.h"
#include "Obj.h"
#include "Type.h"
#include "Err.h"
#include "Str.h"
#include "Bool.h"
#include "Fun.h"
#include "ArgPair.h"
#include "Class.h"
#include "Script.h"
#include "Builtins.h"

void Parser::Parse(const std::string & filePath) {
    std::ifstream t(filePath);
    std::stringstream buffer;
    buffer << t.rdbuf();
    std::string source = buffer.str();
    Interpret(source);
}

void Parser::Interpret(const std::string & source) {
    Tokenizer tokenizer;
    tokenizer.Tokenize(source);

    if (tokenizer.HasError()) {
        std::cout << tokenizer.GetErrorMessage();
        return;
    }

    tokens = tokenizer.GetTokens();
    Ref scriptRef = NEW_PRESERVED_REF(new Script());
    PushDef(scriptRef);
    auto * script = (Script*)GET_OBJ(scriptRef);
    script->self = scriptRef;

    while (CurrentToken()->type != TokenType::EndOfFile) {
        Expr * expr = Parse_Expr();
        if (expr == nullptr)
            continue;
        script->AddExpr(expr);
    }

    Ref result = script->Execute();
    Obj * resultObj = GET_OBJ(result);
    if (resultObj->Is(Err::t)) {
        auto err = (Err*)resultObj;
        std::cout << err->ToStr();
    }
}

void Parser::PushDef(Ref obj) {
    defStack.push_back(obj);
}

Ref Parser::GetLastDef() {
    std::size_t lastIndex = defStack.size() - 1;
    return defStack.at(lastIndex);
}

void Parser::PopDef() {
    defStack.pop_back();
}

Token* Parser::CurrentToken() {
    return tokens.at(currentPosition);
}

int Parser::CurrentLine() {
    return tokens.at(currentPosition)->line;
}

bool Parser::Match(TokenType tokenType) {
    if (CurrentToken()->type == tokenType) {
        currentPosition++;
        return true;
    }
    return false;
}

Expr * Parser::Parse_Expr() {
    if (Match(TokenType::If))
        return Parse_If();

    if (Match(TokenType::For)) {
        if (Match(TokenType::L_Parenthesis))
            return Parse_ForC();

        if (IsForIn())
            return Parse_ForIn();

        return Parse_For();
    }

    if (Match(TokenType::Break))
        return Parse_Break();

    if (Match(TokenType::Skip))
        return new ExprSkip(CurrentLine());

    if (Match(TokenType::Return)) {
        Expr * retBody = Parse_Logical();
        Expr * ret = new ExprReturn(retBody, CurrentLine());
        return ret;
    }

    if (Match(TokenType::Assert))
        return Parse_Assert();

    if (IsFunDef())
        return Parse_FunDef();

    if (IsClassDef())
        return Parse_ClassDef();

    return Parse_Assignment();
}

Expr * Parser::Parse_If() {
    // ********************************************************************
    // [if]  [condition][EnterScope][expr_1][expr_2]...[expr_n][ExitScope]
    //            ^-- we are here
    // [else][condition][EnterScope][expr_1][expr_2]...[expr_n][ExitScope]
    // [else][condition][EnterScope][expr_1][expr_2]...[expr_n][ExitScope]
    // ***
    // [else][EnterScope][expr_1][expr_2]...[expr_n][ExitScope]
    //
    // Notice: else (condition) is treated as "else if" condition.
    //         [else][EnterScope] is treated as ordinary "else",
    //         without any clause.
    // ********************************************************************

    // Parsing condition statement
    auto * exprIf = new ExprIf(CurrentLine());
    Expr * condition = Parse_Logical();
    exprIf->condition = condition;

    // By now empty IFs are not allowed.
    if (CurrentToken()->type != TokenType::EnterScope)
        ReportError("Empty \"if\" statements are not allowed.", CurrentLine());

    currentPosition++;

    // Parsing IF body statements.
    Expr * expr {nullptr};
    while (CurrentToken()->type != TokenType::ExitScope) {
        expr = Parse_Expr();
        exprIf->trueBranch.push_back(expr);
    }
    currentPosition++;

    // Parsing possible ELSE branch statements.
    if (CurrentToken()->type == TokenType::Else) {
        currentPosition++;
        if (CurrentToken()->type == TokenType::EnterScope) {
            // Parsing ordinary 'else' branch.
            currentPosition++;
            while (CurrentToken()->type != TokenType::ExitScope) {
                expr = Parse_Expr();
                exprIf->falseBranch.push_back(expr);
            }
            currentPosition++;
        } else {
            // else (condition) - Parsing 'else if' branch.
            expr = Parse_If();
            exprIf->falseBranch.push_back(expr);
        }
    }
    return exprIf;
}

Expr * Parser::Parse_For() {
    // [for][condition][EnterScope][expr_1][expr_2]...[expr_n][ExitScope]
    //           ^--- we are here

    // Parsing condition statement
    auto * exprFor = new ExprFor(CurrentLine());
    Expr * condition = Parse_Logical();
    exprFor->condition = condition;

    // By now empty 'for' loops are not allowed.
    if (CurrentToken()->type != TokenType::EnterScope)
        ReportError("Empty 'for' loops are not allowed.", CurrentLine());

    currentPosition++;

    // Parsing 'for' loop body statements.
    Expr * expr {nullptr};
    while (CurrentToken()->type != TokenType::ExitScope) {
        expr = Parse_Expr();
        exprFor->expressions.push_back(expr);
    }
    currentPosition++;
    return exprFor;
}

Expr * Parser::Parse_ForC() {
    // ********************************************************************
    // for (initialization; condition; post_statement)
    //           ^-- we are here
    // ********************************************************************

    // Parsing initialization statement
    int savedLine = CurrentLine();
    auto * exprForC = new ExprForC(savedLine);
    Expr * init = Parse_Assignment();
    exprForC->init = init;

    if(!Match(TokenType::Semicolon))
        ReportError("Can't find semicolon after the first statement in the 'for' (C-styled) loop header.", savedLine);

    // Parsing condition statement
    Expr * condition = Parse_Logical();
    exprForC->condition = condition;

    if(!Match(TokenType::Semicolon))
        ReportError("Can't find semicolon after the second statement in the 'for' (C-styled) loop header.", savedLine);

    // Parsing post_statement
    Expr * post = Parse_Assignment();
    exprForC->post = post;

    if(!Match(TokenType::R_Parenthesis))
        ReportError("Can't find closing right brace of the 'for' (C-styled) loop header.", savedLine);

    // By now empty FORs are not allowed.
    if (CurrentToken()->type != TokenType::EnterScope)
        ReportError("Empty 'for' loops are not allowed.", CurrentLine());

    currentPosition++;

    // Parsing FOR's body statements.
    Expr * expr {nullptr};
    while (CurrentToken()->type != TokenType::ExitScope) {
        expr = Parse_Expr();
        exprForC->expressions.push_back(expr);
    }
    currentPosition++;
    return exprForC;
}

bool Parser::IsForIn() {
    // for x in list
    //     ^--- we are at x (identifier)
    int pos = currentPosition;
    if (tokens.at(pos)->type != TokenType::Identifier)
        return false;
    pos++;
    return tokens.at(pos)->type == TokenType::In;
}

Expr * Parser::Parse_ForIn() {
    // for x in list
    //     ^--- we are at x (identifier)
    auto * exprForIn = new ExprForIn(CurrentLine());
    auto * token = CurrentToken();
    exprForIn->name = token->literal;
    currentPosition++;

    if (CurrentToken()->type != TokenType::In)
        ReportError("Internal error 'in' keyword not found.", CurrentLine());
    currentPosition++;

    auto * iterableExpr = Parse_Logical();
    exprForIn->iterableExpr = iterableExpr;

    // By now empty 'for' loops are not allowed.
    if (CurrentToken()->type != TokenType::EnterScope)
        ReportError("Empty 'for' loops are not allowed.", CurrentLine());

    currentPosition++;

    // Parsing 'for' loop body statements.
    Expr * expr {nullptr};
    while (CurrentToken()->type != TokenType::ExitScope) {
        expr = Parse_Expr();
        exprForIn->expressions.push_back(expr);
    }
    currentPosition++;
    return exprForIn;
}

Expr * Parser::Parse_Break() {
    return new ExprBreak(CurrentLine());
}

Expr * Parser::Parse_Assert() {
    int savedLine = CurrentLine();
    auto * args = (ExprArgs*) Parse_Args();
    args->AddExpr(new ExprLiteral(VM::GetConstant_Int(savedLine), savedLine));
    auto * funName = new ExprNameAccess(VM::GetConstant_Str("assert"), savedLine);
    return new ExprCallAccess(funName, args, savedLine);
}

bool Parser::IsFunDef() {
    int pos = currentPosition;
    if (tokens.at(pos)->type != TokenType::Identifier)
        return false;
    pos++;
    if (tokens.at(pos)->type != TokenType::L_Parenthesis)
        return false;
    pos++;
    int numOfOpened = 1;
    while (tokens.at(pos)->type != TokenType::EndOfFile && numOfOpened > 0) {
        if (tokens.at(pos)->type == TokenType::L_Parenthesis)
            numOfOpened++;
        else if (tokens.at(pos)->type == TokenType::R_Parenthesis)
            numOfOpened--;
        pos++;
    }
    return tokens.at(pos)->type == TokenType::EnterScope;
}

Expr * Parser::Parse_FunDef() {
    // fun(x, y)
    //  ^--- we are here
    Ref funName = CurrentToken()->literal;
    auto * fun = new Fun(funName);
    Ref funRef = NEW_PRESERVED_REF(fun);
    fun->SetSelf(funRef);

    auto * parent = GET_OBJ(GetLastDef());
    if (!parent->Is(Script::t))
        ReportError("By now, functions can be defined only in scripts.", CurrentLine());

    ((Script*) parent)->AddChildDef(funName, funRef);
    PushDef(funRef);

    // Parsing declared arguments
    currentPosition += 2;
    while (CurrentToken()->type != TokenType::EndOfFile) {
        // It's possible that function has no arguments.
        if (CurrentToken()->type == TokenType::R_Parenthesis) {
            currentPosition++;
            break;
        }
        if (CurrentToken()->type != TokenType::Identifier)
            ReportError("Can't find argument name in a function declaration.", CurrentLine());
        Ref argName = CurrentToken()->literal;
        Ref argDefaultVal = Ref::none;
        currentPosition++;
        if (CurrentToken()->type == TokenType::Equal) {
            // TODO: Allow only constant expressions for default values.
            currentPosition++;
            argDefaultVal = CurrentToken()->literal;
            currentPosition++;
        }
        fun->AddArgument(argName, argDefaultVal);

        if (CurrentToken()->type != TokenType::Comma) {
            if (CurrentToken()->type != TokenType::R_Parenthesis) {
                ReportError("Can't find closing right bracket.", CurrentLine());
            }
            currentPosition++;
            break;
        }
        currentPosition++;
    }

    // Parsing function body
    if (CurrentToken()->type != TokenType::EnterScope)
        ReportError("Function has no body.", CurrentLine());
    currentPosition++;

    while(CurrentToken()->type != TokenType::ExitScope) {
        Expr * expr = Parse_Expr();
        if (expr == nullptr)
            continue;
        fun->AddExpression(expr);
    }
    currentPosition++;
    PopDef();
    return nullptr;
}

bool Parser::IsClassDef() {
    int pos = currentPosition;
    if (tokens.at(pos)->type != TokenType::Identifier)
        return false;
    pos++;
    /*
    if (tokens.at(pos)->type != TokenType::Class)
        return false;
    pos++;
    */
    return tokens.at(pos)->type == TokenType::EnterScope;
}

Expr * Parser::Parse_ClassDef() {
    // SomeClassName
    //       ^--- we are here
    Ref clName = CurrentToken()->literal;
    auto * cl = new Class(clName);
    Ref clRef = NEW_PRESERVED_REF(cl);
    cl->SetSelf(clRef);

    auto * parent = GET_OBJ(GetLastDef());
    if (!parent->Is(Script::t))
        ReportError("By now, classes can be defined only in scripts.", CurrentLine());

    ((Script*) parent)->AddChildDef(clName, clRef);
    PushDef(clRef);

    // Parsing class body.
    currentPosition += 2;
    while (CurrentToken()->type != TokenType::ExitScope) {
        auto token = CurrentToken();
        if (token->type != TokenType::Identifier)
            ReportError("Class member did not recognized", CurrentLine());
        cl->AddField(token->literal);
        currentPosition++;
    }
    currentPosition++;
    PopDef();
    return nullptr;
}

Expr * Parser::Parse_Assignment() {
    // Possible cases:
    // 1) E -> A ~ T
    // where A is strictly an accessor term.
    //       T is an accessor or an arithmentic/logic term.
    //       ~ is (=, +=, -=, *=, /=, ^=)
    //
    // 2) E -> A
    // No operation i.e. A is just a lonely accessor.
    // In this case this accessor is treated as a declaration of a variable.

    Expr * a = Parse_Accessor();
    int savedLine = CurrentLine();

    Expr * b {nullptr};
    switch (CurrentToken()->type) {
        case TokenType::Equal:
            currentPosition++;
            b = Parse_Logical();
            return new ExprAssign((ExprAccess*)a, b, savedLine);

        case TokenType::PlusEq:
            currentPosition++;
            b = Parse_Logical();
            return new ExprAddEq(a, b, savedLine);

        case TokenType::MinusEq:
            currentPosition++;
            b = Parse_Logical();
            return new ExprSubEq(a, b, savedLine);

        case TokenType::MultEq:
            currentPosition++;
            b = Parse_Logical();
            return new ExprMulEq(a, b, savedLine);

        case TokenType::DivEq:
            currentPosition++;
            b = Parse_Logical();
            return new ExprDivEq(a, b, savedLine);

        case TokenType::PowEq:
            currentPosition++;
            b = Parse_Logical();
            return new ExprPowEq(a, b, savedLine);

        default:
            return a; // Declaration or function call
    }
}

Expr * Parser::Parse_List() {
    // E -> [ A, A, A, ..., A ]
    //      ^ - we are here, exactly on a bracket.
    // where A is an expression or a list.
    assert(Match(TokenType::L_Bracket));
    auto * exprList = new ExprList(CurrentLine());
    if (Match(TokenType::R_Bracket))
        return exprList;
    auto * a = Parse_Logical();
    exprList->exprList.push_back(a);
    while (Match(TokenType::Comma)) {
        a = Parse_Logical();
        exprList->AddExpr(a);
    }
    if (!Match(TokenType::R_Bracket))
        ReportError("Can't find closing right bracket.", CurrentLine());
    return exprList;
}

Expr * Parser::Parse_Args() {
    // E -> ( A, A, A, ..., A )
    //      ^--- we are here
    // where A is an expression or a pair.
    assert(Match(TokenType::L_Parenthesis));
    auto * exprArgs = new ExprArgs(CurrentLine());
    if (Match(TokenType::R_Parenthesis))
        return exprArgs;
    Expr * a = Parse_ArgPair();
    exprArgs->AddExpr(a);
    while (Match(TokenType::Comma)) {
        a = Parse_ArgPair();
        exprArgs->AddExpr(a);
    }
    if (!Match(TokenType::R_Parenthesis)) {
        ReportError("Can't find closing right parenthesis.", CurrentLine());
    }
    return exprArgs;
}

Expr * Parser::Parse_ArgPair() {
    // Detect possible argument-value pair
    if (tokens.at(currentPosition)->type == TokenType::Identifier &&
        tokens.at(currentPosition + 1)->type == TokenType::Equal) {
        int savedLine = CurrentLine();
        Ref name = CurrentToken()->literal;
        currentPosition += 2;
        Expr * valExpr = Parse_Logical();
        Expr * exprPair = new ExprArgPair(name, valExpr, savedLine);
        return exprPair;
    }

    // It's just an expression.
    Expr * a = Parse_Logical(); /*
    auto a_access = std::dynamic_pointer_cast<ExprGet>(a);
    if (a_access != nullptr && a_access->IsSingleNameAccess()) {
        auto argName = a_access->GetName();

        // Parsing possible argument-value pair.
        // E.g. perimeter[length = 1 + 3.0, width = a + b]
        if (CurrentToken()->type == TokenType::Equal) {
            currentPosition++;
            Optr b = Parse_EvalList();
            BUBBLE_UP_IF_ERROR(b)
            return ArgPair::S(argName, b);
        }

        // Parsing possible key-value pair.
        // E.g. ["length" : 1 + 3.0, "width" : a + b]
        if (CurrentToken()->type == TokenType::Colon) {
            currentPosition++;
            Optr b = Parse_List();
            BUBBLE_UP_IF_ERROR(b)
            return KeyValPair::S(argName, b);
        }
        // It's a single name access.
        return a;
    } */
    return a;
}

Expr * Parser::Parse_Logical() {
    // E -> T ~ T ~ T ~ ... ~ T
    // where ~ is (and, or)
    Expr * a = Parse_Equality();
    Token* op = CurrentToken();
    while (op->type == TokenType::And ||
           op->type == TokenType::Or)
    {
        int savedLine = CurrentLine();
        currentPosition++;
        Expr * b = Parse_Equality();
        if (op->type == TokenType::And)
            a = new ExprAnd(a, b, savedLine);
        else
            a = new ExprOr(a, b, savedLine);
        op = CurrentToken();
    }
    return a;
}

Expr * Parser::Parse_Equality() {
    // E -> T (=, !=) T
    Expr * a = Parse_Order();
    Token * op = CurrentToken();
    if (op->type == TokenType::Equal ||
        op->type == TokenType::BangEqual)
    {
        int savedLine = CurrentLine();
        currentPosition++;
        Expr * b = Parse_Order();
        if (op->type == TokenType::Equal)
            a = new ExprEq(a, b, savedLine);
        else
            a = new ExprNotEq(a, b, savedLine);
    }
    return a;
}

Expr * Parser::Parse_Order() {
    // E -> T (>, >=, <, <=) T
    Expr * a = Parse_Add();
    Token * op = CurrentToken();
    if (op->type == TokenType::Greater ||
        op->type == TokenType::GreaterEqual ||
        op->type == TokenType::Less ||
        op->type == TokenType::LessEqual)
    {
        int savedLine = CurrentLine();
        currentPosition++;
        Expr * b = Parse_Add();
        switch(op->type) {
            case TokenType::Greater :      a = new ExprGr(a, b, savedLine);   break;
            case TokenType::GreaterEqual : a = new ExprGrEq(a, b, savedLine); break;
            case TokenType::Less :         a = new ExprLs(a, b, savedLine);   break;
            case TokenType::LessEqual :    a = new ExprLsEq(a, b, savedLine); break;
            default: break;
        }
    }
    return a;
}

Expr * Parser::Parse_Add() {
    // E -> (+, -) T ~ T ~ T ~ ... ~ T
    // where ~ is (+, -)
    // Notice possible leading sign before the first term.
    Expr * a = Parse_Mult();
    Token * op = CurrentToken();
    while (op->type == TokenType::Plus ||
           op->type == TokenType::Minus)
    {
        int savedLine = CurrentLine();
        currentPosition++;
        Expr * b = Parse_Mult();
        if (op->type == TokenType::Plus)
            a = new ExprAdd(a, b, savedLine);
        else
            a = new ExprSub(a, b, savedLine);
        op = CurrentToken();
    }
    return a;
}

Expr * Parser::Parse_Mult() {
    // M -> P ~ P ~ P ~ ... ~ P
    // where ~ is (*, /)
    Expr * a = Parse_Pow();
    Token * op = CurrentToken();
    while (op->type == TokenType::Star ||
           op->type == TokenType::Slash ||
           op->type == TokenType::Percent)
    {
        int savedLine = CurrentLine();
        currentPosition++;
        Expr * b = Parse_Pow();
        switch (op->type) {
            case TokenType::Star :  a = new ExprMul(a, b, savedLine); break;
            case TokenType::Slash : a = new ExprDiv(a, b, savedLine); break;
            default: break;
        }
        op = CurrentToken();
    }
    return a;
}

Expr * Parser::Parse_Pow() {
    // P -> T ^ T ^ T ^ ... ^ T
    Expr * a = Parse_Term();
    Token * op = CurrentToken();
    while (op->type == TokenType::Caret) {
        int savedLine = CurrentLine();
        currentPosition++;
        Expr * b = Parse_Term();
        a = new ExprPow(a, b, savedLine);
        op = CurrentToken();
    }
    return a;
}

Expr * Parser::Parse_Term() {
    Expr * a {nullptr};

    // Process possible parentheses.
    // That means that the parsing term is actually an expression.
    if (CurrentToken()->type == TokenType::L_Parenthesis) {
        currentPosition++;
        a = Parse_Logical();
        if (CurrentToken()->type != TokenType::R_Parenthesis) {
            ReportError("Right parethesis not found.", CurrentLine());
        }
        currentPosition++;
        return a;
    }

    // Process possible list.
    if (CurrentToken()->type == TokenType::L_Bracket) {
        a = Parse_List();
        return a;
    }

    // Process possible 'not' or '-' sign.
    TokenType op = CurrentToken()->type;
    if (op == TokenType::Not || op == TokenType::Minus) {
        int savedLine = CurrentLine();
        currentPosition++;
        if (CurrentToken()->type == TokenType::L_Parenthesis) {
            currentPosition++;
            a = Parse_Logical();
            if (CurrentToken()->type != TokenType::R_Parenthesis) {
                ReportError("Right parethesis not found.", CurrentLine());
            }
            currentPosition++;
        } else {
            a = Parse_Term();
        }
        if (op == TokenType::Not)
            a = new ExprNot(a, savedLine);
        else // Minus
            a = new ExprNeg(a, savedLine);
        return a;
    }

    // Process possible '+' sign. We just skip it.
    while(Match(TokenType::Plus));

    // Literal or identifier
    switch (CurrentToken()->type) {
        case TokenType::Int :
        case TokenType::Real :
        case TokenType::String :
            a = new ExprLiteral(CurrentToken()->literal, CurrentLine());
            currentPosition++;
            break;

        case TokenType::True :
            a = new ExprLiteral(Bool::True, CurrentLine());
            currentPosition++;
            break;

        case TokenType::False :
            a = new ExprLiteral(Bool::False, CurrentLine());
            currentPosition++;
            break;

        case TokenType::Identifier :
            a = Parse_Accessor();
            break;

        default:
            ReportError("Unknown term.", CurrentLine());
    }
    return a;
}

Expr * Parser::Parse_Accessor() {
    // E -> A ~ ~ ~ ~ ... ~
    // where A is an identifier
    // where ~ is a pair of parentheses () or brackets [] or a dot.

    if (CurrentToken()->type != TokenType::Identifier)
        ReportError("Undefined accessor term.", CurrentLine());

    Token * t = CurrentToken();
    Expr * a = new ExprNameAccess(t->literal, CurrentLine());
    currentPosition++;

    t = CurrentToken();
    while (true)
    {
        // Function call
        if (t->type == TokenType::L_Parenthesis) {
            Expr * b = Parse_Args();
            a = new ExprCallAccess(a, b, CurrentLine());
            t = CurrentToken();
            continue;
        }

        // Access by index or key
        /*
        if (t->type == TokenType::L_Bracket) {
            Expr * b = Parse_List();
            a = new ExprCallAccess(a, b, CurrentLine());
            t = CurrentToken();
            continue;
        }
        */

        // Field access
        if (t->type == TokenType::Dot) {
            currentPosition++;
            t = CurrentToken();

            if (t->type != TokenType::Identifier)
                ReportError("Field name required after dot.", CurrentLine());

            a = new ExprFieldAccess(a, t->literal, CurrentLine());
            currentPosition++;
            t = CurrentToken();
            continue;
        }
        break;
    }
    return a;
}

void Parser::ReportError(const std::string & errorMessage, uint line) {
    std::cerr << "Parsing error. Line " << line << ". " << errorMessage << std::endl;
    exit(1);
}
