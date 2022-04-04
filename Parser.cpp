#include "Parser.h"
#include "Obj.h"
#include "Type.h"
#include "Err.h"
#include "Str.h"
#include "Bool.h"
#include "Fun.h"
#include "ArgValPair.h"
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

    while (CurrentToken()->tokenType != TokenType::EndOfFile) {
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
    if (CurrentToken()->tokenType == tokenType) {
        currentPosition++;
        return true;
    }
    return false;
}

Expr * Parser::Parse_Expr() {
    if (Match(TokenType::If))
        return Parse_If();

    if (Match(TokenType::For)) {
        if (Match(TokenType::LeftParenthesis))
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
        Expr * retBody = Parse_List();
        Expr * ret = new ExprReturn(retBody, CurrentLine());
        return ret;
    }

    if (Match(TokenType::Assert))
        return Parse_Assert();

    if (IsFunDef())
        return Parse_FunDef();

    //if (IsClassDef())

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
    if (CurrentToken()->tokenType != TokenType::EnterScope)
        ReportError("Empty \"if\" statements are not allowed.", CurrentLine());

    currentPosition++;

    // Parsing IF body statements.
    Expr * expr {nullptr};
    while (CurrentToken()->tokenType != TokenType::ExitScope) {
        expr = Parse_Expr();
        exprIf->trueBranch.push_back(expr);
    }
    currentPosition++;

    // Parsing possible ELSE branch statements.
    if (CurrentToken()->tokenType == TokenType::Else) {
        currentPosition++;
        if (CurrentToken()->tokenType == TokenType::EnterScope) {
            // Parsing ordinary 'else' branch.
            currentPosition++;
            while (CurrentToken()->tokenType != TokenType::ExitScope) {
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
    if (CurrentToken()->tokenType != TokenType::EnterScope)
        ReportError("Empty 'for' loops are not allowed.", CurrentLine());

    currentPosition++;

    // Parsing 'for' loop body statements.
    Expr * expr {nullptr};
    while (CurrentToken()->tokenType != TokenType::ExitScope) {
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

    if(!Match(TokenType::RightParenthesis))
        ReportError("Can't find closing right brace of the 'for' (C-styled) loop header.", savedLine);

    // By now empty FORs are not allowed.
    if (CurrentToken()->tokenType != TokenType::EnterScope)
        ReportError("Empty 'for' loops are not allowed.", CurrentLine());

    currentPosition++;

    // Parsing FOR's body statements.
    Expr * expr {nullptr};
    while (CurrentToken()->tokenType != TokenType::ExitScope) {
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
    if (tokens.at(pos)->tokenType != TokenType::Identifier)
        return false;
    pos++;
    return tokens.at(pos)->tokenType == TokenType::In;
}

Expr * Parser::Parse_ForIn() {
    // for x in list
    //     ^--- we are at x (identifier)
    auto * exprForIn = new ExprForIn(CurrentLine());
    auto * token = CurrentToken();
    exprForIn->name = token->literal;
    currentPosition++;

    if (CurrentToken()->tokenType != TokenType::In)
        ReportError("Internal error 'in' keyword not found.", CurrentLine());
    currentPosition++;

    auto * iterableExpr = Parse_Logical();
    exprForIn->iterableExpr = iterableExpr;

    // By now empty 'for' loops are not allowed.
    if (CurrentToken()->tokenType != TokenType::EnterScope)
        ReportError("Empty 'for' loops are not allowed.", CurrentLine());

    currentPosition++;

    // Parsing 'for' loop body statements.
    Expr * expr {nullptr};
    while (CurrentToken()->tokenType != TokenType::ExitScope) {
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
    auto * args = (ExprEvalList*)Parse_ArgList();
    args->AddExpr(new ExprLiteral(VM::GetConstant_Int(savedLine), savedLine));
    auto * funName = new ExprNameAccess(VM::GetConstant_Str("assert"), savedLine);
    return new ExprCallAccess(funName, args, savedLine);
}

bool Parser::IsFunDef() {
    int pos = currentPosition;
    if (tokens.at(pos)->tokenType != TokenType::Identifier)
        return false;
    pos++;
    if (tokens.at(pos)->tokenType != TokenType::LeftParenthesis)
        return false;
    pos++;
    int numOfOpened = 1;
    while (tokens.at(pos)->tokenType != TokenType::EndOfFile && numOfOpened > 0) {
        if (tokens.at(pos)->tokenType == TokenType::LeftParenthesis ||
            tokens.at(pos)->tokenType == TokenType::LeftBracket)
            numOfOpened++;
        else if (tokens.at(pos)->tokenType == TokenType::RightParenthesis ||
                 tokens.at(pos)->tokenType == TokenType::RightBracket)
            numOfOpened--;
        pos++;
    }
    return tokens.at(pos)->tokenType == TokenType::EnterScope;
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

    ((Script*)parent)->AddChildDefinition(funName, funRef);
    PushDef(funRef);

    // Parsing declared arguments
    currentPosition += 2;
    while (CurrentToken()->tokenType != TokenType::EndOfFile) {
        // It's possible that function has no arguments.
        if (CurrentToken()->tokenType == TokenType::RightParenthesis) {
            currentPosition++;
            break;
        }
        if (CurrentToken()->tokenType != TokenType::Identifier)
            ReportError("Can't find argument name in a function declaration.", CurrentLine());
        fun->AddArgument(CurrentToken()->literal);
        currentPosition++;
        if (CurrentToken()->tokenType != TokenType::Comma) {
            if (CurrentToken()->tokenType != TokenType::RightParenthesis) {
                ReportError("Can't find closing right bracket.", CurrentLine());
            }
            currentPosition++;
            break;
        }
        currentPosition++;
    }

    // Parsing function body
    if (CurrentToken()->tokenType != TokenType::EnterScope)
        ReportError("Function has no body.", CurrentLine());
    currentPosition++;

    while(CurrentToken()->tokenType != TokenType::ExitScope) {
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
    if (tokens.at(pos)->tokenType != TokenType::Identifier)
        return false;
    pos++;
    if (tokens.at(pos)->tokenType != TokenType::Class)
        return false;
    pos++;
    if (tokens.at(pos)->tokenType != TokenType::EnterScope)
        ReportError("Class declared, but not defined", CurrentLine());
    return true;
}

Expr * Parser::Parse_ClassDef() {}

Expr * Parser::Parse_Assignment() {
//*************************************************************************
// Possible cases:
// ________________________________________________________________________
// 1) E -> A ~ T
// where A is strictly an accessor term.
//       T is an accessor or an arithmentic/logic term.
//       ~ is (=, &=, |=, +=, -=, *=, /=, %=, ^=)
// ________________________________________________________________________
// 2) E -> A
// No operation i.e. A is just a lonely accessor.
// In this case this accessor is treated as a declaration of a variable.
//________________________________________________________________________

    Expr * a = Parse_Accessor();
    int savedLine = CurrentLine();

    Expr * b {nullptr};
    switch (CurrentToken()->tokenType) {
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

        case TokenType::RemEq:
            currentPosition++;
            b = Parse_Logical();
            return new ExprRemEq(a, b, savedLine);

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
// where A is an expression or a list

    if (Match(TokenType::LeftBracket)) {
        auto * evalList = new ExprEvalList(CurrentLine());
        if (Match(TokenType::RightBracket))
            return evalList;
        // EvalList can contain EvalLists as its own elements.
        auto * a = Parse_List();
        evalList->exprList.push_back(a);
        while (Match(TokenType::Comma)) {
            a = Parse_List();
            evalList->AddExpr(a);
        }
        if (!Match(TokenType::RightBracket))
            ReportError("Can't find closing right bracket.", CurrentLine());
        return evalList;
    }
    return Parse_Pair();
}

Expr * Parser::Parse_ArgList() {
// E -> (A, A, A, ..., A)
// where A is an expression or a list

    if (Match(TokenType::LeftParenthesis)) {
        auto * initList = new ExprEvalArgList(CurrentLine());
        if (Match(TokenType::RightParenthesis))
            return initList;
        Expr * a = Parse_List();
        initList->AddExpr(a);
        while (Match(TokenType::Comma)) {
            a = Parse_List();
            initList->AddExpr(a);
        }
        if (!Match(TokenType::RightParenthesis)) {
            ReportError("Can't find closing right parenthesis.", CurrentLine());
        }
        return initList;
    }
    return Parse_Logical();
}

Expr * Parser::Parse_Pair() {
    Expr * a = Parse_Logical(); /*
    auto a_access = std::dynamic_pointer_cast<ExprGet>(a);
    if (a_access != nullptr && a_access->IsSingleNameAccess()) {
        auto argName = a_access->GetName();

        // Parsing possible argument-value pair.
        // E.g. perimeter[length = 1 + 3.0, width = a + b]
        if (CurrentToken()->tokenType == TokenType::Equal) {
            currentPosition++;
            Optr b = Parse_EvalList();
            BUBBLE_UP_IF_ERROR(b)
            return ArgValPair::S(argName, b);
        }

        // Parsing possible key-value pair.
        // E.g. ["length" : 1 + 3.0, "width" : a + b]
        if (CurrentToken()->tokenType == TokenType::Colon) {
            currentPosition++;
            Optr b = Parse_List();
            BUBBLE_UP_IF_ERROR(b)
            return KeyValPair::S(argName, b);
        }
        // It's a single name access.
        return a;
    } */
    // It's an expression.
    return a;
}

Expr * Parser::Parse_Logical() {
// E -> T ~ T ~ T ~ ... ~ T
// where ~ is (and, or, xor)

    Expr * a = Parse_Equality();
    Token* op = CurrentToken();
    while (op->tokenType == TokenType::And ||
           op->tokenType == TokenType::Or  ||
           op->tokenType == TokenType::Xor)
    {
        int savedLine = CurrentLine();
        currentPosition++;
        Expr * b = Parse_Equality();
        if (op->tokenType == TokenType::And)
            a = new ExprAnd(a, b, savedLine);
        else if (op->tokenType == TokenType::Or)
            a = new ExprOr(a, b, savedLine);
        else
            a = new ExprXor(a, b, savedLine);
        op = CurrentToken();
    }
    return a;
}

Expr * Parser::Parse_Equality() {
// E -> T (=, !=) T

    Expr * a = Parse_Order();
    Token * op = CurrentToken();
    if (op->tokenType == TokenType::Equal ||
        op->tokenType == TokenType::BangEqual)
    {
        int savedLine = CurrentLine();
        currentPosition++;
        Expr * b = Parse_Order();
        if (op->tokenType == TokenType::Equal)
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
    if (op->tokenType == TokenType::Greater ||
        op->tokenType == TokenType::GreaterEqual ||
        op->tokenType == TokenType::Less ||
        op->tokenType == TokenType::LessEqual)
    {
        int savedLine = CurrentLine();
        currentPosition++;
        Expr * b = Parse_Add();
        switch(op->tokenType) {
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
    while (op->tokenType == TokenType::Plus ||
           op->tokenType == TokenType::Minus)
    {
        int savedLine = CurrentLine();
        currentPosition++;
        Expr * b = Parse_Mult();
        if (op->tokenType == TokenType::Plus)
            a = new ExprAdd(a, b, savedLine);
        else
            a = new ExprSub(a, b, savedLine);
        op = CurrentToken();
    }
    return a;
}

Expr * Parser::Parse_Mult() {
// M -> P ~ P ~ P ~ ... ~ P
// where ~ is (*, /, %)

    Expr * a = Parse_Pow();
    Token * op = CurrentToken();
    while (op->tokenType == TokenType::Star ||
           op->tokenType == TokenType::Slash ||
           op->tokenType == TokenType::Percent)
    {
        int savedLine = CurrentLine();
        currentPosition++;
        Expr * b = Parse_Pow();
        switch (op->tokenType) {
            case TokenType::Star :    a = new ExprMul(a, b, savedLine); break;
            case TokenType::Slash :   a = new ExprDiv(a, b, savedLine); break;
            case TokenType::Percent : a = new ExprRem(a, b, savedLine); break;
            default: break;
        }
        op = CurrentToken();
    }
    return a;
}

Expr * Parser::Parse_Pow() {
// P -> T ^ T ^ T ^ ... ^ T

    Expr * a = Parse_Term();
    Token* op = CurrentToken();
    while (op->tokenType == TokenType::Caret) {
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
    if (CurrentToken()->tokenType == TokenType::LeftParenthesis) {
        currentPosition++;
        a = Parse_Logical();
        if (CurrentToken()->tokenType != TokenType::RightParenthesis) {
            ReportError("Right parethesis not found.", CurrentLine());
        }
        currentPosition++;
        return a;
    }

    if (CurrentToken()->tokenType == TokenType::LeftBracket) {
        a = Parse_List();
        return a;
    }

    // Process possible 'not' or '-' sign.
    TokenType op = CurrentToken()->tokenType;
    if (op == TokenType::Not || op == TokenType::Minus)
    {
        int savedLine = CurrentLine();
        currentPosition++;
        if (CurrentToken()->tokenType == TokenType::LeftParenthesis) {
            currentPosition++;
            a = Parse_Logical();
            if (CurrentToken()->tokenType != TokenType::RightParenthesis) {
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
    switch (CurrentToken()->tokenType) {
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
// where ~ is a pair of parentheses () or brackets [].

    if (CurrentToken()->tokenType != TokenType::Identifier)
        ReportError("Undefined accessor term.", CurrentLine());

    Token * t = CurrentToken();
    Expr * a = new ExprNameAccess(t->literal, CurrentLine());
    currentPosition++;

    t = CurrentToken();
    while (true)
    {
        // Function call
        if (t->tokenType == TokenType::LeftParenthesis) {
            Expr * b = Parse_ArgList();
            a = new ExprCallAccess(a, b, CurrentLine());
            t = CurrentToken();
            continue;
        }

        // Access by index or key
        /*
        if (t->tokenType == TokenType::LeftBracket) {
            Expr * b = Parse_List();
            a = new ExprCallAccess(a, b, CurrentLine());
            t = CurrentToken();
            continue;
        }
        */

        // Field access
        if (t->tokenType == TokenType::Dot) {
            currentPosition++;
            t = CurrentToken();

            if (t->tokenType != TokenType::Identifier)
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
