#ifndef VIRGO_PARSER_H
#define VIRGO_PARSER_H

#include "Tokenizer.h"
#include "Script.h"

class Parser {
    int                 currentPosition {};
    std::vector<Token*> tokens {};

    Token * CurrentToken();
    int     CurrentLine();
    bool    Match(TokenType tokenType);
    Expr *  Parse_Expr();
    Expr *  Parse_If();
    Expr *  Parse_Assignment();
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
    Script * Parse(std::vector<Token*> tokens_);
};

#endif //VIRGO_PARSER_H
