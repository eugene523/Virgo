#ifndef PROTON_CONTEXT_H
#define PROTON_CONTEXT_H

#include <map>
#include "Mem.h"
#include "IDefObj.h"

struct Context {
    Ref executionOwner;
    std::map<Ref, Ref> variables;

    Context();
    Ref  GetVariable(Ref name);
    void SetVariable(Ref name, Ref val);
    void Mark();
    void Print();
};

#endif //PROTON_CONTEXT_H
