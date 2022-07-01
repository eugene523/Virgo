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

Obj * Int_Eq(Obj * self, Obj * other) {
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

Obj * Int_NotEq(Obj * self, Obj * other) {
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

Obj * Int_Neg(Obj * self) {
    assert(self->Is(Int::t));
    v_int selfVal = ((Int*)self)-> val;
    return (Obj*)Int::New(-selfVal);
}

Obj * Int_Add(Obj * self, Obj * other) {
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

Obj * Int_Sub(Obj * self, Obj * other) {
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

Obj * Int_Mul(Obj * self, Obj * other) {
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

Obj * Int_Div(Obj * self, Obj * other) {
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

Obj * Int_Pow(Obj * self, Obj * other) {
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

Obj * Int_Gr(Obj * self, Obj * other) {
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

Obj * Int_GrEq(Obj * self, Obj * other) {
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

Obj * Int_Ls(Obj * self, Obj * other) {
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

Obj * Int_LsEq(Obj * self, Obj * other) {
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

std::string Int_DebugStr(Obj * self) {
    assert(self->Is(Int::t));
    return std::to_string(((Int*)self)->val);
}

///////////////////////////////////////////////////////////////////////////////

Type * Int::t;

void Int::InitType() {
    Int::t = new Type("int");
    auto mt = Int::t->methodTable;
    mt->Eq       = &Int_Eq;
    mt->NotEq    = &Int_NotEq;
    mt->Neg      = &Int_Neg;
    mt->Add      = &Int_Add;
    mt->Sub      = &Int_Sub;
    mt->Mul      = &Int_Mul;
    mt->Div      = &Int_Div;
    mt->Pow      = &Int_Pow;
    mt->Gr       = &Int_Gr;
    mt->GrEq     = &Int_GrEq;
    mt->Ls       = &Int_Ls;
    mt->LsEq     = &Int_LsEq;
    mt->DebugStr = &Int_DebugStr;
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
