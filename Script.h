#ifndef VIRGO_SCRIPT_H
#define VIRGO_SCRIPT_H

#include <vector>
#include "Expr.h"

class Script {
    ExprScript * exprScript;
    ByteCode bc{};

public:
    explicit Script();
    ~Script();
    void SetExprScript(ExprScript * exprScript_);
    void Compile();
    void Execute();
    void PrintByteCode();
};

#endif //VIRGO_SCRIPT_H
