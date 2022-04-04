#ifndef VIRGO_INVOKER_H
#define VIRGO_INVOKER_H

#include "Obj.h"
#include "Mem.h"

using NMethod = Ref (*) (Obj * self, Ref args);

struct Invoker : Obj {
    Obj * const self;
    const NMethod method;
    static Type * t;
    static void InitType();
    explicit Invoker(Obj * self, NMethod method);
    void Mark() override {};
};

#endif //VIRGO_INVOKER_H
