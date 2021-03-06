#include "Tokenizer.h"
#include "None.h"
#include "Str.h"
#include <utility>

std::map<TokenType, std::string> TokenTypeNames =
{
    { TokenType::Undefined,      "Undefined"       },

    { TokenType::L_Parenthesis,  "L_Parenthesis"   },
    { TokenType::R_Parenthesis,  "R_Parenthesis"   },
    { TokenType::L_Bracket,      "L_Bracket"       },
    { TokenType::R_Bracket,      "R_Bracket"       },

    { TokenType::Comma,          "Comma"           },
    { TokenType::Dot,            "Dot"             },
    { TokenType::Semicolon,      "Semicolon"       },
    { TokenType::Plus,           "Plus"            },
    { TokenType::Minus,          "Minus"           },
    { TokenType::Star,           "Star"            },
    { TokenType::Slash,          "Slash"           },
    { TokenType::Caret,          "Caret"           },

    { TokenType::Equal,          "Equal"           },
    { TokenType::NotEqual,       "NotEqual"        },
    { TokenType::Greater,        "Greater"         },
    { TokenType::GreaterOrEqual, "GreaterOrEqual"  },
    { TokenType::Less,           "Less"            },
    { TokenType::LessOrEqual,    "LessOrEqual"     },

    { TokenType::AddAssign,      "AddAssign"       },
    { TokenType::SubtractAssign, "SubtractAssign"  },
    { TokenType::MultiplyAssign, "MultiplyAssign"  },
    { TokenType::DivideAssign,   "DivideAssign"    },
    { TokenType::PowerAssign,    "PowerAssign"     },

    { TokenType::Int,           "Int"              },
    { TokenType::Real,          "Real"             },
    { TokenType::String,        "String"           },
    { TokenType::Identifier,    "Identifier"       },

    { TokenType::None,          "None"             },
    { TokenType::True,          "True"             },
    { TokenType::False,         "False"            },
    { TokenType::And,           "And"              },
    { TokenType::Or,            "Or"               },
    { TokenType::Not,           "Not"              },
    { TokenType::If,            "If"               },
    { TokenType::Else,          "Else"             },
    { TokenType::For,           "For"              },
    { TokenType::Jump,          "Jump"             },
    { TokenType::Break,         "Break"            },
    { TokenType::Skip,          "Skip"             },
    { TokenType::Return,        "Return"           },
    { TokenType::Assert,        "Assert"           },

    { TokenType::EnterScope,    "EnterScope"       },
    { TokenType::ExitScope,     "ExitScope"        },

    { TokenType::EndOfFile,     "EndOfFile"        },
};

std::string TokenTypeToString(TokenType tokenType) {
    if (TokenTypeNames.count(tokenType) == 0)
        return "Undefined token name.";
    return TokenTypeNames[tokenType];
}

std::string SubStr(const std::string & str, int start, int endExcluded) {
    return str.substr(start, endExcluded - start);
}

void Printw(const std::string & str, int width) {
    std::cout << std::left << std::setw(width) << str;
}

///////////////////////////////////////////////////////////////////////////////

Token::Token(TokenType   tokenType,
             std::string lexeme,
             uint        constantId,
             uint        line)
             :
             type{tokenType},
             lexeme{std::move(lexeme)},
             constantId{constantId},
             constant{VM::GetConstantById(constantId)},
             line{line} {}

Token::Token(TokenType tokenType,
             uint      line)
             :
             type{tokenType},
             lexeme{""},
             constantId{0},
             constant{(Obj*)None::none},
             line{line} {}

///////////////////////////////////////////////////////////////////////////////

Tokenizer::~Tokenizer() {
    for (auto i : tokens) {
        delete i;
    }
}

void Tokenizer::Clear() {
    startPosition       = 0;
    currentPosition     = 0;
    currentLine         = 0;
    n_opened            = 0;
    hasError            = false;
    currentNestingLevel = 0;
    tokens.clear();
}

void Tokenizer::Tokenize(std::string sourceCode) {
    Clear();
    src = std::move(sourceCode);
    ScanTokens();
}

void Tokenizer::DebugTokenize(std::string sourceCode) {
    Tokenize(std::move(sourceCode));
    PrintTokens();
}

bool Tokenizer::HasError() { return hasError; }

std::string Tokenizer::GetErrorMessage() { return errorMessage; }

std::vector<Token*> Tokenizer::GetTokens() { return tokens; }

void Tokenizer::PrintTokens() {
    int width = 20;
    Printw("TokenType", width);
    Printw("Lexeme", width);
    Printw("ConstantId", width);
    Printw("Line", 10);
    std::cout << '\n' << std::string(width * 3 + 10, '-') << "\n";

    uint currLine = 1;
    for (auto i : tokens) {
        if (i->line != currLine) {
            std::cout << std::endl;
            currLine = i->line;
        }
        Printw(TokenTypeToString(i->type), width);
        Printw(i->lexeme, width);
        Printw(i->constantId == 0 ? "" : std::to_string(i->constantId), width);
        Printw(std::to_string(i->line), width);
        std::cout << std::endl;
    }
}

void Tokenizer::ScanTokens() {
    Process_NewLine();
    while (!IsAtEnd()) {
        if (hasError)
            return;
        startPosition = currentPosition;
        ScanToken();
    }
    Process_EndOfFile();
}

bool Tokenizer::IsAtEnd() {
    return currentPosition >= src.size();
}

char Tokenizer::Advance() {
    char c = src.at(currentPosition);
    currentPosition++;
    return c;
}

char Tokenizer::Peek() {
    if (IsAtEnd())
        return '\0';
    return src.at(currentPosition);
}

char Tokenizer::PeekNext() {
    if (currentPosition + 1 >= src.size())
        return '\0';
    return src.at(currentPosition + 1);
}

bool Tokenizer::Match(char expected) {
    if (IsAtEnd() || src.at(currentPosition) != expected)
        return false;
    currentPosition++;
    return true;
}

void Tokenizer::Process_NewLine() {
    currentLine++;

    /*
    If the number of currently opened parentheses, brackets or braces
    is more than zero, that means that expression continues
    and we should skip any whitespaces and return.
    */
    if (n_opened > 0) {
        while (Match(' ')) ;
        return;
    }

    int n_whitespaces = 0;
    while (Match(' '))
        n_whitespaces++;

    if (Peek() == '\n' || IsAtEnd())
        return;

    // Number of whitespaces per level
    const int w = 2;

    if (n_whitespaces % w == 0) {
        int nestingLevel = n_whitespaces / w;
        if (nestingLevel - currentNestingLevel >= 2) {
            ReportError("Nesting error.");
            return;
        }
        if (nestingLevel > currentNestingLevel) {
            AddToken(new Token(TokenType::EnterScope, currentLine));
            currentNestingLevel = nestingLevel;
        } else if (nestingLevel < currentNestingLevel) {
            for (int i = 0; i < (currentNestingLevel - nestingLevel); i++)
                AddToken(new Token(TokenType::ExitScope, currentLine));
            currentNestingLevel = nestingLevel;
        }
    }
    else ReportError("Nesting error.");
}

void Tokenizer::Process_Comment() {
    /*
    One line comment starts with one symbol "#" like this:
    # This is a comment.

    Multiline comment starts with two symbols ("##"),
    and ends with the same two symbols ("##") like this:
    ##
    Write less comments.
    Especially multiline comments.
    ##

    It's possible (but not recommended) to have unclosed
    multiline comment if it ends with EOF.
    */
    if (Peek() == '#') {
        // Multiline comment.
        // Notice that we have already processed one '#" character in global switch loop.
        Advance();
        for (;;) {
            if (IsAtEnd())
                break;
            if (Peek() == '#' && PeekNext() == '#') {
                currentPosition += 2;
                break;
            }
            if (Peek() == '\n')
                currentLine++;
            Advance();
        }
    } else {
        // One line comment
        while (Peek() != '\n' && !IsAtEnd())
            Advance();
    }
}

void Tokenizer::Process_WhiteSpace() {
    // Whitespace between keywords, identifiers, etc.
    while (Match(' ') || Match('\t') || Match('\r'));
}

void Tokenizer::Process_String() {
    while (Peek() != '"' && !IsAtEnd()) {
        if (Peek() == '\n')
            currentLine++;
        Advance();
    }

    if (IsAtEnd()) {
        ReportError("Unterminated string.");
        return;
    }

    Advance();
    std::string lexeme = SubStr(src, startPosition + 1, currentPosition - 1);
    uint constantId = VM::GetConstantId_Str(lexeme);
    AddToken(new Token(TokenType::String, lexeme, constantId, currentLine));
}

void Tokenizer::Process_Number() {
    bool isInt = true;
    while (isdigit(Peek()))
        Advance();

    if (Peek() == '.' && isdigit(PeekNext())) {
        isInt = false;
        Advance();
        while (isdigit(Peek()))
            Advance();
    }

    if (Peek() == 'e' || Peek() == 'E') {
        isInt = false;
        Advance();
        if (isdigit(Peek())) {
            while(isdigit(Peek()))
                Advance();
        } else if ((Peek() == '+' || Peek() == '-') && isdigit(PeekNext())) {
            Advance();
            while(isdigit(Peek()))
                Advance();
        } else {
            ReportError("Wrong exponential notation of a number.");
            return;
        }
    }

    std::string lexeme = SubStr(src, startPosition, currentPosition);
    if (isInt) {
        uint constantId = VM::GetConstantId_Int(std::stoll(lexeme));
        AddToken(new Token(TokenType::Int, lexeme, constantId, currentLine));
    } else {
        uint constantId = VM::GetConstantId_Real(std::stold(lexeme));
        AddToken(new Token(TokenType::Real, lexeme, constantId, currentLine));
    }
}

bool Tokenizer::IsKeyword(const std::string & word) {
    return keywords.count(word) > 0;
}

void Tokenizer::Process_Word() {
    while (isalnum(Peek()) || Peek() == '_')
        Advance();

    std::string word = SubStr(src, startPosition, currentPosition);
    if (IsKeyword(word)) {
        AddToken(new Token(keywords[word], word, 0, currentLine));
        return;
    }

    uint constantId = VM::GetConstantId_Str(word);
    AddToken(new Token(TokenType::Identifier, word, constantId, currentLine));
}

void Tokenizer::Process_EndOfFile() {
    /*
    It is possible that code file ends without exiting
    from nesting scopes ('if', 'for', declarations, etc).
    To ensure that all scopes are closed we add
    additional ExitScope tokens
    according to the current nesting level.
    */
    for (int i = 0; i < currentNestingLevel; i++)
        tokens.push_back(new Token(TokenType::ExitScope, currentLine));
    tokens.push_back(new Token(TokenType::EndOfFile, currentLine));
}

void Tokenizer::ScanToken() {
    char c = Advance();
    switch (c) {
        case '(' :
            n_opened++;
            AddToken(TokenType::L_Parenthesis);
            break;

        case ')' :
            n_opened--;
            AddToken(TokenType::R_Parenthesis);
            break;

        case '[' :
            n_opened++;
            AddToken(TokenType::L_Bracket);
            break;

        case ']' :
            n_opened--;
            AddToken(TokenType::R_Bracket);
            break;

        case ',' :
            AddToken(TokenType::Comma);
            break;

        case '.' :
            AddToken(TokenType::Dot);
            break;

        case ':' :
            AddToken(TokenType::Colon);
            break;

        case ';' :
            AddToken(TokenType::Semicolon);
            break;

        case '+' :
            Match('=') ? AddToken(TokenType::AddAssign) : AddToken(TokenType::Plus);
            break;

        case '-' :
            Match('=') ? AddToken(TokenType::SubtractAssign) : AddToken(TokenType::Minus);
            break;

        case '*' :
            Match('=') ? AddToken(TokenType::MultiplyAssign) : AddToken(TokenType::Star);
            break;

        case '/' :
            Match('=') ? AddToken(TokenType::DivideAssign) : AddToken(TokenType::Slash);
            break;

        case '^' :
            Match('=') ? AddToken(TokenType::PowerAssign) : AddToken(TokenType::Caret);
            break;

        case '=' :
            AddToken(TokenType::Equal);
            break;

        case '!' :
            Match('=') ? AddToken(TokenType::NotEqual) : ReportError("Unexpected character.");
            break;

        case '>' :
            Match('=') ? AddToken(TokenType::GreaterOrEqual) : AddToken(TokenType::Greater);
            break;

        case '<' :
            Match('=') ? AddToken(TokenType::LessOrEqual) : AddToken(TokenType::Less);
            break;

        case '#' :
            Process_Comment();
            break;

        case ' ' :
            Process_WhiteSpace();
            break;

        case '\n' :
            Process_NewLine();
            break;

        case '"' :
            Process_String();
            break;

        default:
        {
            if (isdigit(c))
                Process_Number();
            else if (isalpha(c))
                Process_Word();
            else
                ReportError("Unexpected character.");
            break;
        }
    }
}

void Tokenizer::AddToken(TokenType type) { AddToken(type, 0); }

void Tokenizer::AddToken(TokenType tokenType, uint literalId) {
    std::string lexeme = SubStr(src, startPosition, currentPosition);
    tokens.push_back(new Token(tokenType, lexeme, literalId, currentLine));
}

void Tokenizer::AddToken(Token * token) { tokens.push_back(token); }

void Tokenizer::ReportError(const std::string & message) {
    hasError = true;
    std::stringstream s;
    s << "Line " << currentLine << ". " << message << std::endl;
    errorMessage = s.str();
}
