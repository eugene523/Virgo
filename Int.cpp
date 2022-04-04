#include <cassert>
#include <cmath>
#include <sstream>
#include "Type.h"
#include "Int.h"
#include "Real.h"
#include "Bool.h"
#include "Err.h"
#include "ErrorMessages.h"
#include "Str.h"

Ref Int_OpAdd(Ref a, Ref b) {
    Obj * a_obj = GET_OBJ(a);
    if (a_obj == nullptr)
        return NEW_REF(new Err(ERR_FIRST_ARG_IS_NULL));
    assert(a_obj->Is(Int::t));

    Obj * b_obj = GET_OBJ(b);
    if (b_obj == nullptr)
        return NEW_REF(new Err(ERR_FIRST_ARG_IS_NULL));

    v_int a_val {((Int*)a_obj)->val };
    if (b_obj->Is(Int::t))
    {
        v_int b_val {((Int*)b_obj)->val };
        return NEW_REF(new Int(a_val + b_val));
    }
    else if (b_obj->Is(Real::t))
    {
        v_real b_val {((Real*)b_obj)->val };
        return NEW_REF(new Real(a_val + b_val));
    }
    return NEW_REF(new Err(ERR_OP_WRONG_TYPE));
}

Ref Int_OpSub(Ref a, Ref b) {
    Obj * a_obj = GET_OBJ(a);
    if (a_obj == nullptr)
        return NEW_REF(new Err(ERR_FIRST_ARG_IS_NULL));
    assert(a_obj->Is(Int::t));

    Obj * b_obj = GET_OBJ(b);
    if (b_obj == nullptr)
        return NEW_REF(new Err(ERR_FIRST_ARG_IS_NULL));

    v_int a_val {((Int*)a_obj)->val };
    if (b_obj->Is(Int::t))
    {
        v_int b_val {((Int*)b_obj)->val };
        return NEW_REF(new Int(a_val - b_val));
    }
    else if (b_obj->Is(Real::t))
    {
        v_real b_val {((Real*)b_obj)->val };
        return NEW_REF(new Real(a_val - b_val));
    }
    return NEW_REF(new Err(ERR_OP_WRONG_TYPE));
}

Ref Int_OpMul(Ref a, Ref b) {
    Obj * a_obj = GET_OBJ(a);
    if (a_obj == nullptr)
        return NEW_REF(new Err(ERR_FIRST_ARG_IS_NULL));
    assert(a_obj->Is(Int::t));

    Obj * b_obj = GET_OBJ(b);
    if (b_obj == nullptr)
        return NEW_REF(new Err(ERR_SECOND_ARG_IS_NULL));

    v_int a_val {((Int*)a_obj)->val };
    if (b_obj->Is(Int::t))
    {
        v_int b_val {((Int*)b_obj)->val };
        return NEW_REF(new Int(a_val * b_val));
    }
    else if (b_obj->Is(Real::t))
    {
        v_real b_val {((Real*)b_obj)->val };
        return NEW_REF(new Real(a_val * b_val));
    }
    return NEW_REF(new Err(ERR_OP_WRONG_TYPE));
}

Ref Int_OpDiv(Ref a, Ref b) {
    Obj * a_obj = GET_OBJ(a);
    if (a_obj == nullptr)
        return NEW_REF(new Err(ERR_FIRST_ARG_IS_NULL));
    assert(a_obj->Is(Int::t));

    Obj * b_obj = GET_OBJ(b);
    if (b_obj == nullptr)
        return NEW_REF(new Err(ERR_FIRST_ARG_IS_NULL));

    v_int a_val {((Int*)a_obj)->val };
    if (b_obj->Is(Int::t))
    {
        v_int b_val {((Int*)b_obj)->val };
        if (b_val == 0)
            return NEW_REF(new Err(ERR_DIVISION_BY_ZERO));

        return NEW_REF(new Real(((v_real)a_val) / b_val));
    }
    else if (b_obj->Is(Real::t))
    {
        v_real b_val {((Real*)b_obj)->val };
        if (b_val == 0)
            return NEW_REF(new Err(ERR_DIVISION_BY_ZERO));

        return NEW_REF(new Real(a_val / b_val));
    }
    return NEW_REF(new Err(ERR_OP_WRONG_TYPE));
}

Ref Int_OpRem(Ref a, Ref b) {
    Obj * a_obj = GET_OBJ(a);
    if (a_obj == nullptr)
        return NEW_REF(new Err(ERR_FIRST_ARG_IS_NULL));
    assert(a_obj->Is(Int::t));

    Obj * b_obj = GET_OBJ(b);
    if (b_obj == nullptr)
        return NEW_REF(new Err(ERR_SECOND_ARG_IS_NULL));

    if (!(b_obj->Is(Int::t)))
        return NEW_REF(new Err(ERR_OP_WRONG_TYPE));

    v_int a_val { ((Int*)a_obj)->val };
    v_int b_val { ((Int*)b_obj)->val };
    v_int r = a_val % b_val;
    if (r < 0)
        r += b_val;
    return NEW_REF(new Int(r));
}

Ref Int_OpPow(Ref a, Ref b) {
    Obj * a_obj = GET_OBJ(a);
    if (a_obj == nullptr)
        return NEW_REF(new Err(ERR_FIRST_ARG_IS_NULL));
    assert(a_obj->Is(Int::t));

    Obj * b_obj = GET_OBJ(b);
    if (b_obj == nullptr)
        return NEW_REF(new Err(ERR_FIRST_ARG_IS_NULL));

    v_int a_val {((Int*)a_obj)->val };
    if (b_obj->Is(Int::t)) {
        v_int b_val {((Int*)b_obj)->val };
        return NEW_REF(new Real(powl(a_val, b_val)));
    }
    else if (b_obj->Is(Real::t)) {
        v_real b_val {((Real*)b_obj)->val };
        return NEW_REF(new Real(powl(a_val, b_val)));
    }
    return NEW_REF(new Err(ERR_OP_WRONG_TYPE));
}

Ref Int_OpNeg(Ref a) {
    Obj * a_obj = GET_OBJ(a);
    if (a_obj == nullptr)
        return NEW_REF(new Err(ERR_FIRST_ARG_IS_NULL));
    assert(a_obj->Is(Int::t));
    v_int a_val {((Int*)a_obj)-> val };
    return NEW_REF(new Int(-a_val));
}

Ref Int_OpGr(Ref a, Ref b) {
    Obj * a_obj = GET_OBJ(a);
    if (a_obj == nullptr)
        return NEW_REF(new Err(ERR_FIRST_ARG_IS_NULL));
    assert(a_obj->Is(Int::t));

    Obj * b_obj = GET_OBJ(b);
    if (b_obj == nullptr)
        return NEW_REF(new Err(ERR_SECOND_ARG_IS_NULL));

    v_int a_val {((Int*)a_obj)->val };
    if (b_obj->Is(Int::t)) {
        v_int b_val {((Int*)b_obj)-> val };
        return BOOL_REF(a_val > b_val);
    }
    else if (b_obj->Is(Real::t)) {
        v_real b_val {((Real*)b_obj)->val };
        return BOOL_REF(a_val > b_val);
    }
    return NEW_REF(new Err(ERR_OP_WRONG_TYPE));
}

Ref Int_OpGrEq(Ref a, Ref b) {
    Obj * a_obj = GET_OBJ(a);
    if (a_obj == nullptr)
        return NEW_REF(new Err(ERR_FIRST_ARG_IS_NULL));
    assert(a_obj->Is(Int::t));

    Obj * b_obj = GET_OBJ(b);
    if (b_obj == nullptr)
        return NEW_REF(new Err(ERR_SECOND_ARG_IS_NULL));

    v_int a_val {((Int*)a_obj)->val };
    if (b_obj->Is(Int::t)) {
        v_int b_val {((Int*)b_obj)-> val };
        return BOOL_REF(a_val >= b_val);
    }
    else if (b_obj->Is(Real::t)) {
        v_real b_val {((Real*)b_obj)->val };
        return BOOL_REF(a_val >= b_val);
    }
    return NEW_REF(new Err(ERR_OP_WRONG_TYPE));
}

Ref Int_OpLs(Ref a, Ref b) {
    Obj * a_obj = GET_OBJ(a);
    if (a_obj == nullptr)
        return NEW_REF(new Err(ERR_FIRST_ARG_IS_NULL));
    assert(a_obj->Is(Int::t));

    Obj * b_obj = GET_OBJ(b);
    if (b_obj == nullptr)
        return NEW_REF(new Err(ERR_FIRST_ARG_IS_NULL));

    v_int a_val {((Int*)a_obj)->val };
    if (b_obj->Is(Int::t)) {
        v_int b_val {((Int*)b_obj)-> val };
        return BOOL_REF(a_val < b_val);
    }
    else if (b_obj->Is(Real::t)) {
        v_real b_val {((Real*)b_obj)->val };
        return BOOL_REF(a_val < b_val);
    }
    return NEW_REF(new Err(ERR_OP_WRONG_TYPE));
}

Ref Int_OpLsEq(Ref a, Ref b) {
    Obj * a_obj = GET_OBJ(a);
    if (a_obj == nullptr)
        return NEW_REF(new Err(ERR_FIRST_ARG_IS_NULL));
    assert(a_obj->Is(Int::t));

    Obj * b_obj = GET_OBJ(b);
    if (b_obj == nullptr)
        return NEW_REF(new Err(ERR_FIRST_ARG_IS_NULL));

    v_int a_val {((Int*)a_obj)->val };
    if (b_obj->Is(Int::t)) {
        v_int b_val {((Int*)b_obj)-> val };
        return BOOL_REF(a_val <= b_val);
    }
    else if (b_obj->Is(Real::t)) {
        v_real b_val {((Real*)b_obj)->val };
        return BOOL_REF(a_val <= b_val);
    }
    return NEW_REF(new Err(ERR_OP_WRONG_TYPE));
}

Ref Int_OpEq(Ref a, Ref b) {
    if (a.index == b.index)
        return BOOL_REF(true);

    Obj * a_obj = GET_OBJ(a);
    if (a_obj == nullptr)
        return NEW_REF(new Err(ERR_FIRST_ARG_IS_NULL));
    assert(a_obj->Is(Int::t));

    Obj * b_obj = GET_OBJ(b);
    if (b_obj == nullptr)
        return NEW_REF(new Err(ERR_FIRST_ARG_IS_NULL));

    v_int a_val {((Int*)a_obj)->val };
    if (b_obj->Is(Int::t)) {
        v_int b_val {((Int*)b_obj)-> val };
        return BOOL_REF(a_val == b_val);
    }
    else if (b_obj->Is(Real::t)) {
        v_real b_val {((Real*)b_obj)->val };
        return BOOL_REF(a_val == b_val);
    }
    return BOOL_REF(false);
}

Ref Int_OpNotEq(Ref a, Ref b) {
    if (a.index == b.index)
        return BOOL_REF(false);

    Obj * a_obj = GET_OBJ(a);
    if (a_obj == nullptr)
        return NEW_REF(new Err(ERR_FIRST_ARG_IS_NULL));
    assert(a_obj->Is(Int::t));

    Obj * b_obj = GET_OBJ(b);
    if (b_obj == nullptr)
        return NEW_REF(new Err(ERR_FIRST_ARG_IS_NULL));

    v_int a_val {((Int*)a_obj)->val };
    if (b_obj->Is(Int::t)) {
        v_int b_val {((Int*)b_obj)-> val };
        return BOOL_REF(a_val != b_val);
    }
    else if (b_obj->Is(Real::t)) {
        v_real b_val {((Real*)b_obj)->val };
        return BOOL_REF(a_val != b_val);
    }
    return BOOL_REF(true);
}

Ref Int_ToStr(Ref a) {
    Obj * a_obj = GET_OBJ(a);
    if (a_obj == nullptr)
        return NEW_REF(new Err(ERR_FIRST_ARG_IS_NULL));
    assert(a_obj->Is(Int::t));
    auto val = ((Int*)a_obj)->val;
    std::stringstream s;
    s << val;
    return NEW_REF(new Str(s.str()));
}

///////////////////////////////////////////////////////////////////////////////

Type * Int::t;

void Int::InitType() {
    Int::t = new Type("int");
    auto ot = t->opTable;
    ot->OpAdd   = &Int_OpAdd;
    ot->OpSub   = &Int_OpSub;
    ot->OpMul   = &Int_OpMul;
    ot->OpDiv   = &Int_OpDiv;
    ot->OpRem   = &Int_OpRem;
    ot->OpPow   = &Int_OpPow;
    ot->OpNeg   = &Int_OpNeg;
    ot->OpGr    = &Int_OpGr;
    ot->OpGrEq  = &Int_OpGrEq;
    ot->OpLs    = &Int_OpLs;
    ot->OpLsEq  = &Int_OpLsEq;
    ot->OpEq    = &Int_OpEq;
    ot->OpNotEq = &Int_OpNotEq;
    ot->ToStr   = &Int_ToStr;
}

Int::Int() : Obj{Int::t} {}

Int::Int(v_int value) : Obj{Int::t}, val{value} {}

std::string Int::ToStr() const {
    std::stringstream s;
    s << val;
    return s.str();
}
