#ifndef PROTON_TOKENIZER_H
#define PROTON_TOKENIZER_H

#include <string>
#include <utility>
#include <vector>
#include <iostream>
#include <sstream>
#include <cctype>
#include <map>
#include <iomanip>
#include <fstream>
#include <streambuf>
#include "Obj.h"
#include "VM.h"

enum class TokenType {
    Undefined,

    // Different kinds of brackets
    LeftParenthesis,  // (
    RightParenthesis, // )
    LeftBracket,      // [
    RightBracket,     // ]
    LeftBrace,        // {
    RightBrace,       // }

    // Single character tokens
    Comma,        // ,
    Dot,          // .
    Colon,        // :
    Semicolon,    // ;
    Plus,         // +
    Minus,        // -
    Star,         // *
    Slash,        // /
    Percent,      // %
    Caret,        // ^
    Question,     // ?

    // One or two character tokens
    VertLine,     // |
    Ampersand,    // &
    Equal,        // =
    DoubleEqual,  // ==
    Bang,         // !
    BangEqual,    // !=
    Greater,      // >
    GreaterEqual, // >=
    Less,         // <
    LessEqual,    // <=

    AndEq,        // &=
    OrEq,         // |=
    PlusEq,       // +=
    MinusEq,      // -=
    MultEq,       // *=
    DivEq,        // /=
    RemEq,        // %=
    PowEq,        // ^=

    // Literals
    Int,
    Real,         // 5.1, 0.314e+1
    String,       // "It's Britney, bitch!"
    Identifier,   // SomeVariableName

    // Keywords
    None,
    True,
    False,
    And,
    Or,
    Xor,
    Not,
    Assert,
    If,
    Else,
    For,
    In,
    Break,
    Skip,
    Return,
    Class,

    // Scope tokens
    EnterScope,
    ExitScope,

    // End of file token
    EndOfFile
};

std::string TokenTypeToString(TokenType tokenType);

void Printw(const std::string & str, int width);

std::string SubStr(const std::string & str, int start, int endExcluded);

///////////////////////////////////////////////////////////////////////////////

struct Token {
    const TokenType tokenType {};

    // 'lexeme' is a piece of text, that represents some entity.
    // An entity can be:
    //    some constant values: '123', '3.57e+12', "It's Britney, bitch!", 'variableName'.
    //    keywords: 'if', 'else', 'for', etc.
    //    some symbols like braces, brackets, slashes, etc.
    const std::string lexeme;

    // If 'lexeme' represents some constant value like integer, real, string,
    // then 'literal' contains an actual object (Int, Real, Str, etc.),
    // otherwise it's Ref::none.
    const Ref literal;

    // Line of code that contains this token.
    const uint line {};

    explicit Token(TokenType   tokenType,
                   std::string lexeme = "",
                   Ref         literal = Ref::none,
                   uint        line = 0);
};

///////////////////////////////////////////////////////////////////////////////

class Tokenizer {
// The purpose of a Tokenizer is to take a source code presented as a one giant string
// and produce a list of tokens.
    std::string src;
    int         startPosition       {};
    int         currentPosition     {};
    int         currentLine         {};
    int         n_opened            {}; // Number of currently opened parentheses, brackets or braces.
    bool        hasError            {false};
    std::string errorMessage        {};
    int         currentNestingLevel {};

    std::vector<Token*> tokens {};

    std::map<std::string, TokenType> keywords =
    {
        {"none",   TokenType::None},
        {"true",   TokenType::True},
        {"false",  TokenType::False},
        {"and",    TokenType::And},
        {"or",     TokenType::Or},
        {"xor",    TokenType::Xor},
        {"not",    TokenType::Not},
        {"assert", TokenType::Assert},
        {"if",     TokenType::If},
        {"else",   TokenType::Else},
        {"for",    TokenType::For},
        {"in",     TokenType::In},
        {"break",  TokenType::Break},
        {"skip",   TokenType::Skip},
        {"return", TokenType::Return},
        {"class",  TokenType::Class},
    };

    void ScanTokens();
    bool IsAtEnd();
    char Advance();
    char Peek();
    char PeekNext();
    bool Match(char expected);
    void Process_NewLine();
    void Process_Comment();
    void Process_WhiteSpace();
    void Process_String();
    void Process_Number();
    bool IsKeyword(const std::string & word);
    void Process_Word();
    void Process_EndOfFile();
    void ScanToken();
    void AddToken(TokenType type);
    void AddToken(TokenType tokenType, Ref literal);
    void AddToken(Token * token);
    void ReportError(const std::string & message);

public:
    void Clear();
    void Tokenize(std::string sourceCode);
    void DebugTokenize(std::string sourceCode);
    bool HasError();
    std::string GetErrorMessage();
    std::vector<Token*> GetTokens();
    void PrintTokens();
    ~Tokenizer();
};

#endif //PROTON_TOKENIZER_H
