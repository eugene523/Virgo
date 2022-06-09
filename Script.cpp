#include "Script.h"
#include "VM.h"

Script::Script() = default;

Script::~Script() {
    for (auto e : expressions) {
        delete e;
    }
}

void Script::AddExpr(Expr * expr) {
    expressions.push_back(expr);
}

void Script::Execute() {
    ByteCode bc;
    bc.Write_NewFrame();
    for (size_t i = 0; i < expressions.size(); i++) {
        expressions[i]->Compile(bc);
    }
    //bc.Write_CloseFrame();
    VM::Execute(bc);
}



