#ifndef PROTON_PARSER_H
#define PROTON_PARSER_H

#include "Tokenizer.h"
#include "Expr_.h"

class Parser {
    int                 currentPosition {};
    std::vector<Token*> tokens {};
    std::vector<Ref>    defStack;

    void    PushDef(Ref obj);
    Ref     GetLastDef();
    void    PopDef();
    Token * CurrentToken();
    int     CurrentLine();
    bool    Match(TokenType tokenType);
    Expr *  Parse_Expr();
    Expr *  Parse_If();
    Expr *  Parse_For();
    Expr *  Parse_ForC();
    bool    IsForIn();
    Expr *  Parse_ForIn();
    Expr *  Parse_Break();
    Expr *  Parse_Assert();
    bool    IsFunDef();
    Expr *  Parse_FunDef();
    bool    IsClassDef();
    Expr *  Parse_ClassDef();
    Expr *  Parse_Assignment();
    Expr *  Parse_List();
    Expr *  Parse_Args();
    Expr *  Parse_ArgPair();
    Expr *  Parse_Logical();
    Expr *  Parse_Equality();
    Expr *  Parse_Order();
    Expr *  Parse_Add();
    Expr *  Parse_Mult();
    Expr *  Parse_Pow();
    Expr *  Parse_Term();
    Expr *  Parse_Accessor();
    static void ReportError(const std::string & errorMessage, uint line);
public:
    void Parse(const std::string & filePath);
    void Interpret(const std::string & source);
};

#endif //PROTON_PARSER_H
