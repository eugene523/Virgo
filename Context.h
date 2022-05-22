#ifndef VIRGO_CTX_H
#define VIRGO_CTX_H

#include <map>
#include "Obj.h"

struct Context {
    std::map<Obj*, Obj*> variables;

    Obj * GetVariable(Obj * name);
    Obj * SetVariable(Obj * name, Obj * value);
    void  Print();
};

#endif //VIRGO_CTX_H
