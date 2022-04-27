#include <cassert>
#include <cmath>
#include <sstream>
#include "Type.h"
#include "Bool.h"
#include "Int.h"
#include "Real.h"
#include "Err.h"
#include "ErrorMessages.h"

Obj * Real_OpAdd(Obj * self, Obj * other) {
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
    return (Obj*)Err::New(ERR_OP_WRONG_TYPE);
}

Obj * Real_OpSub(Obj * self, Obj * other) {
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
    return (Obj*)Err::New(ERR_OP_WRONG_TYPE);
}

Obj * Real_OpMult(Obj * self, Obj * other) {
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
    return (Obj*)Err::New(ERR_OP_WRONG_TYPE);
}

Obj * Real_OpDiv(Obj * self, Obj * other) {
    assert(self->Is(Real::t));
    v_real selfVal = ((Real*)self)->val;
    if (other->Is(Real::t))
    {
        v_real otherVal = ((Real*)other)->val;
        if (otherVal == 0)
            return (Obj*)Err::New(ERR_DIVISION_BY_ZERO);
        return (Obj*)Real::New(selfVal / otherVal);
    }
    else if (other->Is(Int::t))
    {
        v_int otherVal = ((Int*)other)->val;
        if (otherVal == 0)
            return (Obj*)Err::New(ERR_DIVISION_BY_ZERO);
        return (Obj*)Real::New(selfVal / otherVal);
    }
    return (Obj*)Err::New(ERR_OP_WRONG_TYPE);
}

Obj * Real_OpPow(Obj * self, Obj * other) {
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
    return (Obj*)Err::New(ERR_OP_WRONG_TYPE);
}

Obj * Real_OpNeg(Obj * self) {
    assert(self->Is(Real::t));
    v_real selfVal = ((Real*)self)->val;
    return (Obj*)Real::New(-selfVal);
}

Obj * Real_OpGr(Obj * self, Obj * other) {
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
    return (Obj*)Err::New(ERR_OP_WRONG_TYPE);
}

Obj * Real_OpGrEq(Obj * self, Obj * other) {
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
    return (Obj*)Err::New(ERR_OP_WRONG_TYPE);
}

Obj * Real_OpLs(Obj * self, Obj * other) {
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
    return (Obj*)Err::New(ERR_OP_WRONG_TYPE);
}

Obj * Real_OpLsEq(Obj * self, Obj * other) {
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
    return (Obj*)Err::New(ERR_OP_WRONG_TYPE);
}

Obj * Real_OpEq(Obj * self, Obj * other) {
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
    return (Obj*)Err::New(ERR_OP_WRONG_TYPE);
}

Obj * Real_OpNotEq(Obj * self, Obj * other) {
    if (self == other)
        return (Obj*)Bool::New(false);

    assert(self->Is(Real::t));
    v_real selfVal = ((Real*)self)->val;
    if (other->Is(Real::t))
    {
        v_real otherVal = ((Real*)other)-> val;
        return (Obj*)Bool::New(selfVal != otherVal);
    }
    else if (other->Is(Int::t))
    {
        v_int otherVal = ((Int*)other)->val;
        return (Obj*)Bool::New(selfVal != otherVal);
    }
    return (Obj*)Err::New(ERR_OP_WRONG_TYPE);
}

///////////////////////////////////////////////////////////////////////////////

Type * Real::t;

void Real::InitType() {
    Real::t = new Type("real");
    auto mt = t->methodTable;
    mt->OpAdd   = &Real_OpAdd;
    mt->OpSub   = &Real_OpSub;
    mt->OpMul   = &Real_OpMult;
    mt->OpDiv   = &Real_OpDiv;
    mt->OpPow   = &Real_OpPow;
    mt->OpNeg   = &Real_OpNeg;
    mt->OpGr    = &Real_OpGr;
    mt->OpGrEq  = &Real_OpGrEq;
    mt->OpLs    = &Real_OpLs;
    mt->OpLsEq  = &Real_OpLsEq;
    mt->OpEq    = &Real_OpEq;
    mt->OpNotEq = &Real_OpNotEq;
}

Real * Real::New(v_real value) {
    Real * r = (Real*)Heap::GetChunk(sizeof(Real));
    Obj::Init(r, Real::t);
    r->val = value;
    return r;
}

/*
std::string Real::ToStr() const {
    std::stringstream s;
    s << val;
    return s.str();
}
 */
