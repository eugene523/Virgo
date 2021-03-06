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
    L_Parenthesis,  // (
    R_Parenthesis,  // )
    L_Bracket,      // [
    R_Bracket,      // ]

    // Single character tokens
    Comma,          // ,
    Dot,            // .
    Colon,          // :
    Semicolon,      // ;
    Plus,           // +
    Minus,          // -
    Star,           // *
    Slash,          // /
    Caret,          // ^

    // One or two character tokens
    Equal,          // =
    NotEqual,       // !=
    Greater,        // >
    GreaterOrEqual, // >=
    Less,           // <
    LessOrEqual,    // <=

    AddAssign,      // +=
    SubtractAssign, // -=
    MultiplyAssign, // *=
    DivideAssign,   // /=
    PowerAssign,    // ^=

    // Literals
    Int,
    Real,           // 5.1, 0.314e+1
    String,         // "It's Britney, bitch!"
    Identifier,     // SomeVariableName

    // Keywords
    None,
    True,
    False,
    And,
    Or,
    Not,
    If,
    Else,
    For,
    In,
    Jump,
    Break,
    Skip,
    Return,
    Assert,

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
    const TokenType type;

    /*
    'lexeme' is a piece of text, that represents some entity.
    An entity can be:
        some constant values: '123', '3.57e+12', "It's Britney, bitch!", 'variableName'.
        keywords: 'if', 'else', 'for', etc.
        some symbols like braces, brackets, slashes, etc.
    */
    const std::string lexeme;

    // If 'lexeme' represents some constant value like integer, real, string,
    // then 'constant' contains an actual object (Int, Real, Str, etc.),
    // otherwise it is 'none'.
    const uint  constantId;
    const Obj * constant;

    // Line of code that contains this token.
    const uint line {};

    explicit Token(TokenType   tokenType,
                   std::string lexeme,
                   uint        constantId,
                   uint        line);

    explicit Token(TokenType tokenType,
                   uint      line);
};

///////////////////////////////////////////////////////////////////////////////

class Tokenizer {
    std::string src;
    int         startPosition       {};
    int         currentPosition     {};
    int         currentLine         {};
    int         n_opened            {}; // Number of currently opened parentheses, brackets or braces.
    bool        hasError            {false};
    std::string errorMessage        {};
    int         currentNestingLevel {};

    std::vector<Token*> tokens;

    std::map<std::string, TokenType> keywords =
    {
        {"none",   TokenType::None},
        {"true",   TokenType::True},
        {"false",  TokenType::False},
        {"and",    TokenType::And},
        {"or",     TokenType::Or},
        {"not",    TokenType::Not},
        {"if",     TokenType::If},
        {"else",   TokenType::Else},
        {"for",    TokenType::For},
        {"in",     TokenType::In},
        {"jump",   TokenType::Jump},
        {"break",  TokenType::Break},
        {"skip",   TokenType::Skip},
        {"return", TokenType::Return},
        {"assert", TokenType::Assert},
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
    void AddToken(TokenType tokenType, uint literalId);
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
