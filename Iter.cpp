#include "Iter.h"
#include "Type.h"
#include "List.h"
#include "Seg.h"
#include "Err.h"

Type * Iter::t;

void Iter::InitType() {
    Iter::t = new Type("Iter");
}

Iter::Iter(Ref target) : Obj{Iter::t}, target{target} {
    auto * targetObj = GET_OBJ(target);
    if (targetObj->Is(Seg::t))
        this->target = ((Seg*)targetObj)->ToList();
}

bool Iter::IsAvailable() {
    auto * targetObj = GET_OBJ(target);
    return targetObj->Is(List::t);
}

bool Iter::HasNext() {
    auto * listObj = (List*)(GET_OBJ(target));
    return currentIndex < listObj->elements.size();
}

Ref Iter::Next() {
    auto * listObj = (List*)(GET_OBJ(target));
    Ref ret = listObj->elements.at(currentIndex);
    currentIndex++;
    return ret;
}

void Iter::Mark() {
    Mem::MarkRef(target);
    Obj * obj = GET_OBJ(target);
    obj->Mark();
}
