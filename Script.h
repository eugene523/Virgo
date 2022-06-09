#ifndef VIRGO_SCRIPT_H
#define VIRGO_SCRIPT_H

#include <vector>
#include "Expr.h"

class Script {
    std::vector<Expr*> expressions;

public:
    explicit Script();
    ~Script();
    void AddExpr(Expr * expr);
    void Execute();
};

#endif //VIRGO_SCRIPT_H
