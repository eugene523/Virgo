#include <cassert>
#include <cmath>
#include <sstream>
#include "Type.h"
#include "Bool.h"
#include "Int.h"
#include "Real.h"
#include "Error.h"
#include "ErrorMessages.h"

Obj * Real_Equal(Obj * self, Obj * other) {
    if (self == other)
        return (Obj*)Bool::New(true);

    assert(self->Is(Real::t));
    v_real selfVal = ((Real*)self)->val;
    if (other->Is(Real::t))
    {
        v_real otherVal = ((Real*)other)-> val;
        return (Obj*)Bool::New(selfVal == otherVal);
    }
    else if (other->Is(Int::t))
    {
        v_int otherVal = ((Int*)other)->val;
        return (Obj*)Bool::New(selfVal == otherVal);
    }
    return (Obj*)Error::New(ERROR_INCOMPATIBLE_TYPES);
}

Obj * Real_Neg(Obj * self) {
    assert(self->Is(Real::t));
    v_real selfVal = ((Real*)self)->val;
    return (Obj*)Real::New(-selfVal);
}

Obj * Real_Add(Obj * self, Obj * other) {
    assert(self->Is(Real::t));
    v_real selfVal = ((Real*)self)->val;
    if (other->Is(Real::t))
    {
        v_real otherVal = ((Real*)other)->val;
        return (Obj*)Real::New(selfVal + otherVal);
    }
    else if (other->Is(Int::t))
    {
        v_int otherVal = ((Int*)other)->val;
        return (Obj*)Real::New(selfVal + otherVal);
    }
    return (Obj*)Error::New(ERROR_INCOMPATIBLE_TYPES);
}

Obj * Real_Sub(Obj * self, Obj * other) {
    assert(self->Is(Real::t));
    v_real selfVal = ((Real*)self)->val;
    if (other->Is(Real::t))
    {
        v_real otherVal = ((Real*)other)->val;
        return (Obj*)Real::New(selfVal - otherVal);
    }
    else if (other->Is(Int::t))
    {
        v_int otherVal = ((Int*)other)->val;
        return (Obj*)Real::New(selfVal - otherVal);
    }
    return (Obj*)Error::New(ERROR_INCOMPATIBLE_TYPES);
}

Obj * Real_Mul(Obj * self, Obj * other) {
    assert(self->Is(Real::t));
    v_real selfVal = ((Real*)self)->val;
    if (other->Is(Real::t))
    {
        v_real otherVal = ((Real*)other)->val;
        return (Obj*)Real::New(selfVal * otherVal);
    }
    else if (other->Is(Int::t))
    {
        v_int otherVal = ((Int*)other)->val;
        return (Obj*)Real::New(selfVal * otherVal);
    }
    return (Obj*)Error::New(ERROR_INCOMPATIBLE_TYPES);
}

Obj * Real_Div(Obj * self, Obj * other) {
    assert(self->Is(Real::t));
    v_real selfVal = ((Real*)self)->val;
    if (other->Is(Real::t))
    {
        v_real otherVal = ((Real*)other)->val;
        if (otherVal == 0)
            return (Obj*)Error::New(ERROR_DIVISION_BY_ZERO);
        return (Obj*)Real::New(selfVal / otherVal);
    }
    else if (other->Is(Int::t))
    {
        v_int otherVal = ((Int*)other)->val;
        if (otherVal == 0)
            return (Obj*)Error::New(ERROR_DIVISION_BY_ZERO);
        return (Obj*)Real::New(selfVal / otherVal);
    }
    return (Obj*)Error::New(ERROR_INCOMPATIBLE_TYPES);
}

Obj * Real_Pow(Obj * self, Obj * other) {
    assert(self->Is(Real::t));
    v_real selfVal = ((Real*)self)->val;
    if (other->Is(Real::t))
    {
        v_real otherVal = ((Real*)other)->val;
        return (Obj*)Real::New(powl(selfVal, otherVal));
    }
    else if (other->Is(Int::t))
    {
        v_int otherVal = ((Int*)other)->val;
        return (Obj*)Real::New(powl(selfVal, otherVal));
    }
    return (Obj*)Error::New(ERROR_INCOMPATIBLE_TYPES);
}

Obj * Real_Gr(Obj * self, Obj * other) {
    assert(self->Is(Real::t));
    v_real selfVal = ((Real*)self)->val;
    if (other->Is(Real::t))
    {
        v_real otherVal = ((Real*)other)->val;
        return (Obj*)Bool::New(selfVal > otherVal);
    }
    else if (other->Is(Int::t))
    {
        v_int otherVal = ((Int*)other)->val;
        return (Obj*)Bool::New(selfVal > otherVal);
    }
    return (Obj*)Error::New(ERROR_INCOMPATIBLE_TYPES);
}

Obj * Real_GrEq(Obj * self, Obj * other) {
    assert(self->Is(Real::t));
    v_real selfVal = ((Real*)self)->val;
    if (other->Is(Real::t))
    {
        v_real otherVal = ((Real*)other)->val;
        return (Obj*)Bool::New(selfVal >= otherVal);
    }
    else if (other->Is(Int::t))
    {
        v_int otherVal = ((Int*)other)->val;
        return (Obj*)Bool::New(selfVal >= otherVal);
    }
    return (Obj*)Error::New(ERROR_INCOMPATIBLE_TYPES);
}

Obj * Real_Ls(Obj * self, Obj * other) {
    assert(self->Is(Real::t));
    v_real selfVal = ((Real*)self)->val;
    if (other->Is(Real::t))
    {
        v_real otherVal = ((Real*)other)->val;
        return (Obj*)Bool::New(selfVal < otherVal);
    }
    else if (other->Is(Int::t))
    {
        v_int otherVal = ((Int*)other)->val;
        return (Obj*)Bool::New(selfVal < otherVal);
    }
    return (Obj*)Error::New(ERROR_INCOMPATIBLE_TYPES);
}

Obj * Real_LsEq(Obj * self, Obj * other) {
    assert(self->Is(Real::t));
    v_real selfVal = ((Real*)self)->val;
    if (other->Is(Real::t))
    {
        v_real otherVal = ((Real*)other)-> val;
        return (Obj*)Bool::New(selfVal <= otherVal);
    }
    else if (other->Is(Int::t))
    {
        v_int otherVal = ((Int*)other)->val;
        return (Obj*)Bool::New(selfVal <= otherVal);
    }
    return (Obj*)Error::New(ERROR_INCOMPATIBLE_TYPES);
}

std::string Real_DebugStr(Obj * self) {
    assert(self->Is(Real::t));
    return std::to_string(((Real*)self)->val);
}

///////////////////////////////////////////////////////////////////////////////

Type * Real::t;

void Real::InitType() {
    Real::t = new Type("real");
    auto mt = t->methodTable;
    mt->Equal    = &Real_Equal;
    mt->Neg      = &Real_Neg;
    mt->Add      = &Real_Add;
    mt->Sub      = &Real_Sub;
    mt->Mul      = &Real_Mul;
    mt->Div      = &Real_Div;
    mt->Pow      = &Real_Pow;
    mt->Gr       = &Real_Gr;
    mt->GrEq     = &Real_GrEq;
    mt->Ls       = &Real_Ls;
    mt->LsEq     = &Real_LsEq;
    mt->DebugStr = &Real_DebugStr;
}

void Real::New(void * inPlace, v_real value) {
    Real * r = (Real*)inPlace;
    Obj::Init(r, Real::t);
    r->val = value;
}

Real * Real::New(v_real value) {
    Real * r = (Real*) Heap::GetChunk_Baby(sizeof(Real));
    Obj::Init(r, Real::t);
    r->val = value;
    return r;
}
