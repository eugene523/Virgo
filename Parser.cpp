#include "Parser.h"

#include <utility>

Script * Parser::Parse(std::vector<Token*> tokens_) {
    tokens = std::move(tokens_);
    auto * script = new Script();
    while (CurrentToken()->type != TokenType::EndOfFile) {
        Expr * expr = Parse_Expr();
        assert(expr != nullptr);
        script->AddExpr(expr);
    }
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
    return Parse_Assignment();
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
            //return new ExprSetLocalVariable((ExprAccess*)a, b, savedLine);
            return new ExprSetLocalVariable(((ExprGetLocalVariable*)a)->id, b, savedLine);
/*
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
*/
        default:
            return a; // Declaration or function call
    }
}

Expr * Parser::Parse_Logical() {
    // E -> T ~ T ~ T ~ ... ~ T
    // where ~ is (and, or)
    Expr * a = Parse_Equality();
    /*
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
     */
    return a;
}

Expr * Parser::Parse_Equality() {
    // E -> T (=, !=) T
    Expr * a = Parse_Order();
    /*
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
     */
    return a;
}

Expr * Parser::Parse_Order() {
    // E -> T (>, >=, <, <=) T
    Expr * a = Parse_Add();
    /*
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
     */
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

    /*
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
     */

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
    // E -> A ~ ~ ~ ~ ... ~
    // where A is an identifier
    // where ~ is a pair of parentheses () or brackets [] or just a dot.

    if (CurrentToken()->type != TokenType::Identifier)
        ReportError("Undefined accessor term.", CurrentLine());

    Token * t = CurrentToken();
    Expr * a = new ExprGetLocalVariable(t->constantId, CurrentLine());
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
