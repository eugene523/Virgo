#include <cassert>
#include "Type.h"
#include "Invoker.h"

Ref Invoker_Get(Ref invokerRef, Ref args) {
    Obj * i = GET_OBJ(invokerRef);
    assert(i->Is(Invoker::t));
    auto * invoker = (Invoker*)i;
    return invoker->method(invoker->self, args);
}

Type * Invoker::t;

void Invoker::InitType() {
    Invoker::t = new Type("Invoker");
    auto ot = t->opTable;
    ot->Get = &Invoker_Get;
}

Invoker::Invoker(Obj * self, NMethod method) :
Obj{Invoker::t}, self{self}, method{method} {}