#ifndef VIRGO_SCRIPT_H
#define VIRGO_SCRIPT_H

#include <vector>
#include "Expr.h"

class Script {
    std::vector<Expr*> expressions;
    ByteCode bc{};

public:
    explicit Script();
    ~Script();
    void AddExpr(Expr * expr);
    void Compile();
    void Execute();
    void PrintByteCode();
};

#endif //VIRGO_SCRIPT_H
