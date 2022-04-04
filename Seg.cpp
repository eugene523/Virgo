#include <cassert>
#include <sstream>
#include "Seg.h"
#include "Type.h"
#include "Int.h"
#include "Str.h"
#include "List.h"

Type * Seg::t;

void Seg::InitType() {
    Seg::t = new Type("seg");
}

Seg::Seg(Ref a, Ref b, Ref step) :
Obj{Seg::t}, a{a}, b{b}, step{step} {}

Ref Seg::ToList() {
    auto * aObj = GET_OBJ(a);
    assert(aObj->Is(Int::t));
    v_int aInt { ((Int*)aObj)->val };

    auto * bObj = GET_OBJ(b);
    assert(bObj->Is(Int::t));
    v_int bInt { ((Int*)bObj)->val };

    auto * stepObj = GET_OBJ(step);
    assert(stepObj->Is(Int::t));
    v_int incInt { ((Int*)stepObj)->val };

    List * list = new List();
    for (v_int x = aInt; x < bInt; x += incInt)
        list->elements.push_back(NEW_REF(new Int(x)));
    
    return NEW_REF(list);
}

std::string Seg::ToStr() const {
    std::stringstream s;
    s << "seg(" << GET_OBJ(a)->ToStr() << ", " << GET_OBJ(b)->ToStr() << ")";
    return s.str();
}

void Seg::Mark() {
    Mem::MarkRef(a);
    auto * obj = GET_OBJ(a);
    obj->Mark();

    Mem::MarkRef(b);
    obj = GET_OBJ(b);
    obj->Mark();

    Mem::MarkRef(step);
    obj = GET_OBJ(step);
    obj->Mark();
}
