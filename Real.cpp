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

Obj * Real_Negate(Obj * self) {
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

Obj * Real_Subtract(Obj * self, Obj * other) {
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

Obj * Real_Multiply(Obj * self, Obj * other) {
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

Obj * Real_Divide(Obj * self, Obj * other) {
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

Obj * Real_Power(Obj * self, Obj * other) {
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

Obj * Real_Greater(Obj * self, Obj * other) {
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

Obj * Real_GreaterOrEqual(Obj * self, Obj * other) {
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

Obj * Real_Less(Obj * self, Obj * other) {
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

Obj * Real_LessOrEqual(Obj * self, Obj * other) {
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
    mt->Equal          = &Real_Equal;
    mt->Negate         = &Real_Negate;
    mt->Add            = &Real_Add;
    mt->Subtract       = &Real_Subtract;
    mt->Multiply       = &Real_Multiply;
    mt->Divide         = &Real_Divide;
    mt->Power          = &Real_Power;
    mt->Greater        = &Real_Greater;
    mt->GreaterOrEqual = &Real_GreaterOrEqual;
    mt->Less           = &Real_Less;
    mt->LessOrEqual    = &Real_LessOrEqual;
    mt->DebugStr       = &Real_DebugStr;
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
