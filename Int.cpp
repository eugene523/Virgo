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
    return (Obj*)Error::New(ERR_OP_WRONG_TYPE);
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
    return (Obj*)Error::New(ERR_OP_WRONG_TYPE);
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
    return (Obj*)Error::New(ERR_OP_WRONG_TYPE);
}

Obj * Int_OpDiv(Obj * self, Obj * other) {
    assert(self->Is(Int::t));
    v_int selfVal = ((Int*)self)->val;
    if (other->Is(Int::t))
    {
        v_int otherVal = ((Int*)other)->val;
        if (otherVal == 0)
            return (Obj*)Error::New(ERR_DIVISION_BY_ZERO);
        
        return (Obj*)Real::New(((v_real)selfVal) / otherVal);
    }
    else if (other->Is(Real::t))
    {
        v_real otherVal = ((Real*)other)->val;
        if (otherVal == 0)
            return (Obj*)Error::New(ERR_DIVISION_BY_ZERO);

        return (Obj*)Real::New(selfVal / otherVal);
    }
    return (Obj*)Error::New(ERR_OP_WRONG_TYPE);
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
    return (Obj*)Error::New(ERR_OP_WRONG_TYPE);
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
    return (Obj*)Error::New(ERR_OP_WRONG_TYPE);
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
    return (Obj*)Error::New(ERR_OP_WRONG_TYPE);
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
    return (Obj*)Error::New(ERR_OP_WRONG_TYPE);
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
    return (Obj*)Error::New(ERR_OP_WRONG_TYPE);
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

/*
Obj * Int_ToStr(Obj * self) {
    
    if (self == nullptr)
        return (Obj*)Error::New(ERR_FIRST_ARG_IS_NULL));
    assert(self->Is(Int::t));
    auto val = ((Int*)self)->val;
    std::stringstream s;
    s << val;
    return NEW_REF(new Str(s.str()));
}
 */

///////////////////////////////////////////////////////////////////////////////

Type * Int::t;

void Int::InitType() {
    Int::t = new Type("int");
    auto mtab = Int::t->methodTable;
    mtab->OpAdd   = &Int_OpAdd;
    mtab->OpSub   = &Int_OpSub;
    mtab->OpMul   = &Int_OpMul;
    mtab->OpDiv   = &Int_OpDiv;
    mtab->OpPow   = &Int_OpPow;
    mtab->OpNeg   = &Int_OpNeg;
    mtab->OpGr    = &Int_OpGr;
    mtab->OpGrEq  = &Int_OpGrEq;
    mtab->OpLs    = &Int_OpLs;
    mtab->OpLsEq  = &Int_OpLsEq;
    mtab->OpEq    = &Int_OpEq;
    mtab->OpNotEq = &Int_OpNotEq;
    //mtab->ToStr   = &Int_ToStr;
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
