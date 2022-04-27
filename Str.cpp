#include <cassert>
#include <utility>
#include "Type.h"
#include "Str.h"
#include "Bool.h"
#include "Int.h"
#include "Args.h"
#include "Err.h"
#include "ErrorMessages.h"

Obj * Str_OpAdd(Obj * self, Obj * other) {
    assert(self->Is(Str::t));
    std::string selfVal = ((Str*)self)->val;
    if (other->Is(Str::t))
    {
        std::string otherVal = ((Str*)other)->val;
        return (Obj*)Str::New(selfVal + otherVal);
    }
    return (Obj*)Err::New(ERR_OP_WRONG_TYPE);
}

Obj * Str_OpEq(Obj * self, Obj * other) {
    if (self == other)
        return (Obj*)Bool::True;

    assert(self->Is(Str::t));
    std::string selfVal = ((Str*)self)->val;
    if (other->Is(Str::t))
    {
        std::string otherVal = ((Str*)other)->val;
        return (Obj*)Bool::New(selfVal == otherVal);
    }
    return (Obj*)Err::New(ERR_OP_WRONG_TYPE);
}

Obj * Str_OpNotEq(Obj * self, Obj * other) {
    Obj * self = GET_OBJ(self);
    if (self == nullptr)
        return NEW_REF(new Err(ERR_FIRST_ARG_IS_NULL));
    assert(self->Is(Str::t));

    Obj * other = GET_OBJ(other);
    if (other == nullptr)
        return NEW_REF(new Err(ERR_SECOND_ARG_IS_NULL));

    std::string selfVal = ((Str*)self)->val;
    if (other->Is(Str::t)) {
        std::string otherVal = ((Str*)other)->val;
        return BOOL_REF(selfVal != otherVal);
    }
    return (Obj*)Err::New(ERR_OP_WRONG_TYPE);
}

Obj * Str_Get(Obj * self, Obj * other) {
    Obj * self = GET_OBJ(self);
    if (self == nullptr)
        return NEW_REF(new Err(ERR_FIRST_ARG_IS_NULL));
    assert(self->Is(Str::t));
    Str * str = (Str*)self;

    Obj * other = GET_OBJ(other);
    if (other == nullptr)
        return NEW_REF(new Err(ERR_SECOND_ARG_IS_NULL));
    assert(other->Is(Args::t));
    auto * argsObj = (Args*)other;
    Obj * indexRef = argsObj->Get(0);
    Obj * indexObj = GET_OBJ(indexRef);
    if (indexObj->Is(Int::t)) {
        Int * index = (Int*)indexObj;
        if (index->val < 0 || index->val >= str->val.length())
            return NEW_REF(new Err("Index is out of range."));
        return (Obj*)Str::New(str->val[index->val]));
    }
    return NEW_REF(new Err("Index must be self positive integer."));
}

///////////////////////////////////////////////////////////////////////////////

Type * Str::t;

void Str::InitType() {
    Str::t = new Type("str");
    auto ot = Str::t->opTable;
    ot->OpAdd   = &Str_OpAdd;
    ot->OpEq    = &Str_OpEq;
    ot->OpNotEq = &Str_OpNotEq;
    ot->Get     = &Str_Get;
}

Str::Str(std::string val) : Obj{Str::t}, val{std::move(val)} {}

Str::Str(char val) : Obj{Str::t}, val{val} {}

std::string Str::ToStr() const {
    return val;
}
