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

Obj * Int_Equal(Obj * self, Obj * other) {
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

Obj * Int_Negate(Obj * self) {
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

Obj * Int_Subtract(Obj * self, Obj * other) {
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

Obj * Int_Multiply(Obj * self, Obj * other) {
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

Obj * Int_Divide(Obj * self, Obj * other) {
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

Obj * Int_Power(Obj * self, Obj * other) {
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

Obj * Int_Greater(Obj * self, Obj * other) {
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

Obj * Int_GreaterOrEqual(Obj * self, Obj * other) {
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

Obj * Int_Less(Obj * self, Obj * other) {
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

Obj * Int_LessOrEqual(Obj * self, Obj * other) {
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
    mt->Equal          = &Int_Equal;
    mt->Negate         = &Int_Negate;
    mt->Add            = &Int_Add;
    mt->Subtract       = &Int_Subtract;
    mt->Multiply       = &Int_Multiply;
    mt->Divide         = &Int_Divide;
    mt->Power          = &Int_Power;
    mt->Greater        = &Int_Greater;
    mt->GreaterOrEqual = &Int_GreaterOrEqual;
    mt->Less           = &Int_Less;
    mt->LessOrEqual    = &Int_LessOrEqual;
    mt->DebugStr       = &Int_DebugStr;
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
