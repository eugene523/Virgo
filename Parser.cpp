#include "Parser.h"

#include <utility>

Script * Parser::Parse(std::vector<Token*> tokens_) {
    tokens = std::move(tokens_);
    auto * exprScript = new ExprScript();
    while (CurrentToken()->type != TokenType::EndOfFile) {
        Expr * expr = Parse_Expr();
        assert(expr != nullptr);
        exprScript->AddExpr(expr);
    }
    auto * script = new Script();
    script->SetExprScript(exprScript);
    return script;
}

Token * Parser::CurrentToken() {
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
        if (Is_For_CStyled())
            return Parse_For_CStyled();

        return Parse_For_Ordinary();
    }

    if (Match(TokenType::Break))
        return Parse_Break();

    if (Match(TokenType::Skip))
        return Parse_Skip();

    if (IsLabel())
        return Parse_Label();

    if (Match(TokenType::Jump))
        return Parse_Jump();

    if (Match(TokenType::Assert))
        return Parse_Assert();

    return Parse_Assignment();
}

Expr * Parser::Parse_If() {
/*
    [if]  [condition][EnterScope][expr_1][expr_2]...[expr_n][ExitScope]
               ^-- we are here
    [else][condition][EnterScope][expr_1][expr_2]...[expr_n][ExitScope]
    [else][condition][EnterScope][expr_1][expr_2]...[expr_n][ExitScope]
    ***
    [else][EnterScope][expr_1][expr_2]...[expr_n][ExitScope]

    Notice:
    [else][condition] is treated as "else if" condition.
    [else][EnterScope] is treated as ordinary "else", without any clause.
*/

    // Parsing condition statement
    auto * exprIf = new ExprIf(CurrentLine());
    exprIf->SetCondition(Parse_Logical());

    if (CurrentToken()->type != TokenType::EnterScope)
        ReportError("Empty 'if' statements are not allowed.", CurrentLine());

    currentPosition++;

    // Parsing IF body statements.
    Expr * expr = nullptr;
    while (CurrentToken()->type != TokenType::ExitScope) {
        expr = Parse_Expr();
        exprIf->AddTrueExpr(expr);
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
                exprIf->AddFalseExpr(expr);
            }
            currentPosition++;
        } else {
            // else (condition) - Parsing 'else if' branch.
            expr = Parse_If();
            exprIf->AddFalseExpr(expr);
        }
    }
    return exprIf;
}

Expr * Parser::Parse_For_Ordinary() {
/*
    [for][condition][EnterScope][expr_1][expr_2]...[expr_n][ExitScope]
              ^--- we are here
*/
    // Parsing condition statement
    auto * exprFor = new ExprFor(CurrentLine());
    exprFor->forType = ForType::Ordinary;
    exprFor->SetCondition(Parse_Logical());

    // Empty 'for' loops are not allowed.
    if (CurrentToken()->type != TokenType::EnterScope)
        ReportError("Empty 'for' loops are not allowed.", CurrentLine());

    currentPosition++;

    // Parsing 'for' loop body statements.
    while (CurrentToken()->type != TokenType::ExitScope) {
        auto * expr = Parse_Expr();
        exprFor->AddBodyExpr(expr);
    }
    currentPosition++;
    return exprFor;
}

bool Parser::Is_For_CStyled() {
/*
    for (initialization ; condition ; iteration)
        ^--- we are here, at possible left parenthesis
*/
    int pos = currentPosition;
    if (tokens.at(pos)->type != TokenType::L_Parenthesis)
        return false;
    pos++;

    int numOfOpened = 1;
    int numOfSemicolons = 0;
    while (tokens.at(pos)->type != TokenType::EndOfFile && numOfOpened > 0) {
        if (tokens.at(pos)->type == TokenType::Semicolon)
            numOfSemicolons++;
        else if (tokens.at(pos)->type == TokenType::L_Parenthesis)
            numOfOpened++;
        else if (tokens.at(pos)->type == TokenType::R_Parenthesis)
            numOfOpened--;
        pos++;
    }

    if (numOfSemicolons == 0)
        return false;

    if (numOfSemicolons != 2)
        ReportError("Wrong 'for (C styled)' syntax. Condition section must have two semicolons.", CurrentLine());

    return true;
}

Expr * Parser::Parse_For_CStyled() {
/*
    for (initialization ; condition ; iteration)
        ^-- we are here, at possible left parenthesis
*/
    int savedLine = CurrentLine();
    assert(Match(TokenType::L_Parenthesis));

    auto * exprForC = new ExprFor(savedLine);
    exprForC->forType = ForType::CStyled;

    // Parsing initialization statement
    Expr * init = Parse_Assignment();
    exprForC->AddInitExpr(init);

    if(!Match(TokenType::Semicolon))
        ReportError("Can't find semicolon after the first statement in the 'for (C styled)' loop header.", savedLine);

    // Parsing condition
    Expr * condition = Parse_Logical();
    exprForC->SetCondition(condition);

    if(!Match(TokenType::Semicolon))
        ReportError("Can't find semicolon after the second statement in the 'for (C styled)' loop header.", savedLine);

    // Parsing iteration
    Expr * post = Parse_Assignment();
    exprForC->AddIterExpr(post);

    if(!Match(TokenType::R_Parenthesis))
        ReportError("Can't find closing right brace of the 'for (C styled)' loop header.", savedLine);

    // Empty loops are not allowed.
    if (!Match(TokenType::EnterScope))
        ReportError("Empty 'for' loops are not allowed.", CurrentLine());

    // Parsing body statements.
    Expr * expr {nullptr};
    while (CurrentToken()->type != TokenType::ExitScope) {
        expr = Parse_Expr();
        exprForC->AddBodyExpr(expr);
    }
    currentPosition++;
    return exprForC;
}

Expr * Parser::Parse_Break() {
    return new ExprBreak(CurrentLine());
}

Expr * Parser::Parse_Skip() {
    return new ExprSkip(CurrentLine());
}

bool Parser::IsLabel() {
    return (tokens.at(currentPosition)->type     == TokenType::Identifier &&
            tokens.at(currentPosition + 1)->type == TokenType::Colon);
}

Expr * Parser::Parse_Label() {
/*
    labelName:
        ^--- we are here
*/
    int savedLine = CurrentLine();
    auto * idToken = CurrentToken();
    assert(idToken->type == TokenType::Identifier);
    std::string labelName = idToken->lexeme;
    currentPosition++;
    assert(Match(TokenType::Colon));
    return new ExprLabel(labelName, savedLine);
}

Expr * Parser::Parse_Jump() {
/*
    jump(labelName)
        ^--- we are here at left parenthesis
*/
    std::string errMsg = "Wrong 'jump' syntax. "
                         "Proper usage is: 'jump(labelName)'.";

    int savedLine = CurrentLine();

    if (!Match(TokenType::L_Parenthesis))
        ReportError(errMsg, savedLine);

    auto * idToken = CurrentToken();
    if (idToken->type != TokenType::Identifier)
        ReportError(errMsg, savedLine);

    std::string labelName = idToken->lexeme;
    currentPosition++;

    if (!Match(TokenType::R_Parenthesis))
        ReportError(errMsg, savedLine);

    return new ExprJump(labelName, savedLine);
}

Expr * Parser::Parse_Assert() {
/*
    assert(expression,["error message"])
          ^--- we are here

    Notice that "error message" is optional.
*/
    std::string errMsg = "Wrong 'assert' syntax. "
                         "Proper usage is 'assert(expression,['error message'])'.";

    int  savedLine   = CurrentLine();
    uint savedLineId = VM::GetConstantId_Int(savedLine);
    uint messageId   = 0;

    if (!Match(TokenType::L_Parenthesis))
        ReportError(errMsg, savedLine);

    Expr * checkingExpr = Parse_Logical();
    auto * tok = CurrentToken();
    if (tok->type == TokenType::Comma) {
        currentPosition++;
        tok = CurrentToken();
        if (tok->type != TokenType::String)
            ReportError(errMsg, savedLine);
        messageId = tok->constantId;
        currentPosition++;
    }

    if (!Match(TokenType::R_Parenthesis))
        ReportError(errMsg, savedLine);

    return new ExprAssert(checkingExpr, savedLine, savedLineId, messageId);
}

Expr * Parser::Parse_Assignment() {
/*
    Possible cases:
    1) E -> A ~ T
    where A is strictly an accessor term.
          T is an accessor or an arithmentic/logic term.
          ~ is (=, +=, -=, *=, /=, ^=)

    2) E -> A
    No operation i.e. A is just a lonely accessor.
    In this case this accessor is treated as a declaration of a variable.
*/

    Expr * a = Parse_Accessor();
    int savedLine = CurrentLine();

    Expr * b = nullptr;
    switch (CurrentToken()->type) {
        case TokenType::Equal:
            currentPosition++;
            b = Parse_Logical();
            assert(a->exprType == ExprType::Dot);
            ((ExprDot*)a)->SetValueExpr(b);
            return a;

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

Expr * Parser::Parse_Logical() {
/*
    E -> T ~ T ~ T ~ ... ~ T
    where ~ is (and, or)
*/
    Expr * a = Parse_Equality();
    Token * op = CurrentToken();
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
/*
    E -> T ~ T
    where ~ is (=, !=)
*/
    Expr * a = Parse_Order();
    Token * op = CurrentToken();
    if (op->type == TokenType::Equal ||
        op->type == TokenType::BangEqual)
    {
        int savedLine = CurrentLine();
        currentPosition++;
        Expr * b = Parse_Order();
        if (op->type == TokenType::Equal)
            a = new ExprEqual(a, b, savedLine);
        else
            a = new ExprNotEqual(a, b, savedLine);
    }
    return a;
}

Expr * Parser::Parse_Order() {
/*
    E -> T ~ T
    where ~ is (>, >=, <, <=)
*/
    Expr * a = Parse_Add();
    Token * op = CurrentToken();
    if (op->type == TokenType::Greater      ||
        op->type == TokenType::GreaterEqual ||
        op->type == TokenType::Less         ||
        op->type == TokenType::LessEqual)
    {
        int savedLine = CurrentLine();
        currentPosition++;
        Expr * b = Parse_Add();
        switch(op->type) {
            case TokenType::Greater:
                a = new ExprGr(a, b, savedLine);
                break;

            case TokenType::GreaterEqual:
                a = new ExprGrEq(a, b, savedLine);
                break;

            case TokenType::Less:
                a = new ExprLs(a, b, savedLine);
                break;

            case TokenType::LessEqual:
                a = new ExprLsEq(a, b, savedLine);
                break;

            default:
                break;
        }
    }
    return a;
}

Expr * Parser::Parse_Add() {
/*
    E -> (+, -) T ~ T ~ T ~ ... ~ T
    where ~ is (+, -)
    Notice possible leading sign before the first term.
*/
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
/*
    M -> P ~ P ~ P ~ ... ~ P
    where ~ is (*, /)
*/
    Expr * a = Parse_Pow();
    Token * op = CurrentToken();
    while (op->type == TokenType::Star ||
           op->type == TokenType::Slash)
    {
        int savedLine = CurrentLine();
        currentPosition++;
        Expr * b = Parse_Pow();

        if (op->type == TokenType::Star)
            a = new ExprMul(a, b, savedLine);
        else
            a = new ExprDiv(a, b, savedLine);

        op = CurrentToken();
    }
    return a;
}

Expr * Parser::Parse_Pow() {
/*
    P -> T ^ T ^ T ^ ... ^ T
*/
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
    Expr * a = nullptr;

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

    /*
    // Process possible list.
    if (CurrentToken()->type == TokenType::L_Bracket) {
        a = Parse_List();
        return a;
    }
     */

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
            a = new ExprLoadConstant(CurrentToken()->constantId, CurrentLine());
            currentPosition++;
            break;

        case TokenType::True :
            a = new ExprLoadConstant(VM::TrueId, CurrentLine());
            currentPosition++;
            break;

        case TokenType::False :
            a = new ExprLoadConstant(VM::FalseId, CurrentLine());
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
/*
    E -> A ~ ~ ~ ~ ... ~
    where A is an identifier
    where ~ is a pair of parentheses () or brackets [] or just a dot.
*/

    auto * tok = CurrentToken();
    if (tok->type != TokenType::Identifier)
        ReportError("Undefined accessor term.", CurrentLine());

    Expr * a = new ExprDot(tok->constantId, CurrentLine());
    currentPosition++;

//    t = CurrentToken();
//    while (true)
//    {
//        // Function call
//        if (t->type == TokenType::L_Parenthesis) {
//            Expr * b = Parse_Args();
//            a = new ExprCallAccess(a, b, CurrentLine());
//            t = CurrentToken();
//            continue;
//        }
//
//        // Access by index or key
//        /*
//        if (t->type == TokenType::L_Bracket) {
//            Expr * b = Parse_List();
//            a = new ExprCallAccess(a, b, CurrentLine());
//            t = CurrentToken();
//            continue;
//        }
//        */
//
//        // Field access
//        if (t->type == TokenType::Dot) {
//            currentPosition++;
//            t = CurrentToken();
//
//            if (t->type != TokenType::Identifier)
//                ReportError("Field name required after a dot.", CurrentLine());
//
//            a = new ExprFieldAccess(a, t->literal, CurrentLine());
//            currentPosition++;
//            t = CurrentToken();
//            continue;
//        }
//        break;
//    }
    return a;
}

void Parser::ReportError(const std::string & errorMessage, uint line) {
    std::cerr << "Syntax error. Line " << line << ". " << errorMessage << std::endl;
    exit(1);
}
