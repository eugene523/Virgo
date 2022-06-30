#include <cassert>
#include <cmath>
#include <sstream>
#include "Type.h"
#include "Int.h"
#include "Real.h"
#include "Bool.h"
#include "Error.h"
#include "ErrorMessages.h"
#include "Str.h"

Obj * Int_OpAdd(Obj * self, Obj * other) {
    assert(self->Is(Int::t));
    v_int selfVal = ((Int*)self)->val;
    if (other->Is(Int::t))
    {
        v_int otherVal = ((Int*)other)->val;
        return (Obj*)Int::New(selfVal + otherVal);
    }
    else if (other->Is(Real::t))
    {
        v_real otherVal = ((Real*)other)->val;
        return (Obj*)Real::New(selfVal + otherVal);
    }
    return (Obj*)Error::New(ERROR_INCOMPATIBLE_TYPES);
}

Obj * Int_OpSub(Obj * self, Obj * other) {
    assert(self->Is(Int::t));
    v_int selfVal = ((Int*)self)->val;
    if (other->Is(Int::t))
    {
        v_int otherVal = ((Int*)other)->val;
        return (Obj*)Int::New(selfVal - otherVal);
    }
    else if (other->Is(Real::t))
    {
        v_real otherVal = ((Real*)other)->val;
        return (Obj*)Real::New(selfVal - otherVal);
    }
    return (Obj*)Error::New(ERROR_INCOMPATIBLE_TYPES);
}

Obj * Int_OpMul(Obj * self, Obj * other) {
    assert(self->Is(Int::t));
    v_int selfVal = ((Int*)self)->val;
    if (other->Is(Int::t))
    {
        v_int otherVal = ((Int*)other)->val;
        return (Obj*)Int::New(selfVal * otherVal);
    }
    else if (other->Is(Real::t))
    {
        v_real otherVal = ((Real*)other)->val;
        return (Obj*)Real::New(selfVal * otherVal);
    }
    return (Obj*)Error::New(ERROR_INCOMPATIBLE_TYPES);
}

Obj * Int_OpDiv(Obj * self, Obj * other) {
    assert(self->Is(Int::t));
    v_int selfVal = ((Int*)self)->val;
    if (other->Is(Int::t))
    {
        v_int otherVal = ((Int*)other)->val;
        if (otherVal == 0)
            return (Obj*)Error::New(ERROR_DIVISION_BY_ZERO);

        return (Obj*)Real::New(((v_real)selfVal) / otherVal);
    }
    else if (other->Is(Real::t))
    {
        v_real otherVal = ((Real*)other)->val;
        if (otherVal == 0)
            return (Obj*)Error::New(ERROR_DIVISION_BY_ZERO);

        return (Obj*)Real::New(selfVal / otherVal);
    }
    return (Obj*)Error::New(ERROR_INCOMPATIBLE_TYPES);
}

Obj * Int_OpPow(Obj * self, Obj * other) {
    assert(self->Is(Int::t));
    v_int selfVal = ((Int*)self)->val;
    if (other->Is(Int::t))
    {
        v_int otherVal = ((Int*)other)->val;
        return (Obj*)Real::New(powl(selfVal, otherVal));
    }
    else if (other->Is(Real::t))
    {
        v_real otherVal = ((Real*)other)->val;
        return (Obj*)Real::New(powl(selfVal, otherVal));
    }
    return (Obj*)Error::New(ERROR_INCOMPATIBLE_TYPES);
}

Obj * Int_OpNeg(Obj * self) {
    assert(self->Is(Int::t));
    v_int selfVal = ((Int*)self)-> val;
    return (Obj*)Int::New(-selfVal);
}

Obj * Int_OpGr(Obj * self, Obj * other) {
    assert(self->Is(Int::t));
    v_int selfVal = ((Int*)self)->val;
    if (other->Is(Int::t))
    {
        v_int otherVal = ((Int*)other)-> val;
        return (Obj*)Bool::New(selfVal > otherVal);
    }
    else if (other->Is(Real::t))
    {
        v_real otherVal = ((Real*)other)->val;
        return (Obj*)Bool::New(selfVal > otherVal);
    }
    return (Obj*)Error::New(ERROR_INCOMPATIBLE_TYPES);
}

Obj * Int_OpGrEq(Obj * self, Obj * other) {
    assert(self->Is(Int::t));
    v_int selfVal = ((Int*)self)->val;
    if (other->Is(Int::t))
    {
        v_int otherVal = ((Int*)other)-> val;
        return (Obj*)Bool::New(selfVal >= otherVal);
    }
    else if (other->Is(Real::t))
    {
        v_real otherVal = ((Real*)other)->val;
        return (Obj*)Bool::New(selfVal >= otherVal);
    }
    return (Obj*)Error::New(ERROR_INCOMPATIBLE_TYPES);
}

Obj * Int_OpLs(Obj * self, Obj * other) {
    assert(self->Is(Int::t));
    v_int selfVal = ((Int*)self)->val;
    if (other->Is(Int::t))
    {
        v_int otherVal = ((Int*)other)-> val;
        return (Obj*)Bool::New(selfVal < otherVal);
    }
    else if (other->Is(Real::t))
    {
        v_real otherVal = ((Real*)other)->val;
        return (Obj*)Bool::New(selfVal < otherVal);
    }
    return (Obj*)Error::New(ERROR_INCOMPATIBLE_TYPES);
}

Obj * Int_OpLsEq(Obj * self, Obj * other) {
    assert(self->Is(Int::t));
    v_int selfVal = ((Int*)self)->val;
    if (other->Is(Int::t))
    {
        v_int otherVal = ((Int*)other)-> val;
        return (Obj*)Bool::New(selfVal <= otherVal);
    }
    else if (other->Is(Real::t))
    {
        v_real otherVal = ((Real*)other)->val;
        return (Obj*)Bool::New(selfVal <= otherVal);
    }
    return (Obj*)Error::New(ERROR_INCOMPATIBLE_TYPES);
}

Obj * Int_OpEq(Obj * self, Obj * other) {
    if (self == other)
        return (Obj*)Bool::New(true);

    assert(self->Is(Int::t));
    v_int selfVal = ((Int*)self)->val;
    if (other->Is(Int::t))
    {
        v_int otherVal = ((Int*)other)-> val;
        return (Obj*)Bool::New(selfVal == otherVal);
    }
    else if (other->Is(Real::t))
    {
        v_real otherVal = ((Real*)other)->val;
        return (Obj*)Bool::New(selfVal == otherVal);
    }
    return (Obj*)Bool::New(false);
}

Obj * Int_OpNotEq(Obj * self, Obj * other) {
    if (self == other)
        return (Obj*)Bool::New(false);

    assert(self->Is(Int::t));
    v_int selfVal = ((Int*)self)->val;
    if (other->Is(Int::t))
    {
        v_int otherVal = ((Int*)other)-> val;
        return (Obj*)Bool::New(selfVal != otherVal);
    }
    else if (other->Is(Real::t))
    {
        v_real otherVal = ((Real*)other)->val;
        return (Obj*)Bool::New(selfVal != otherVal);
    }
    return (Obj*)Bool::New(true);
}

std::string Int_Dstr(Obj * self) {
    assert(self->Is(Int::t));
    return std::to_string(((Int*)self)->val);
}

///////////////////////////////////////////////////////////////////////////////

Type * Int::t;

void Int::InitType() {
    Int::t = new Type("int");
    auto mt = Int::t->methodTable;
    mt->OpAdd   = &Int_OpAdd;
    mt->OpSub   = &Int_OpSub;
    mt->OpMul   = &Int_OpMul;
    mt->OpDiv   = &Int_OpDiv;
    mt->OpPow   = &Int_OpPow;
    mt->OpNeg   = &Int_OpNeg;
    mt->OpGr    = &Int_OpGr;
    mt->OpGrEq  = &Int_OpGrEq;
    mt->OpLs    = &Int_OpLs;
    mt->OpLsEq  = &Int_OpLsEq;
    mt->OpEq    = &Int_OpEq;
    mt->OpNotEq = &Int_OpNotEq;
    mt->Dstr    = &Int_Dstr;
}

void Int::New(void * inPlace, v_int value) {
    Int * i = (Int*)inPlace;
    Obj::Init(i, Int::t);
    i->val = value;
}

Int * Int::New(v_int value) {
    Int * i = (Int*)Heap::GetChunk_Baby(sizeof(Int));
    Obj::Init(i, Int::t);
    i->val = value;
    return i;
}
