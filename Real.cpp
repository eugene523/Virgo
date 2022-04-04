#include <cassert>
#include <cmath>
#include <sstream>
#include "Type.h"
#include "Bool.h"
#include "Int.h"
#include "Real.h"
#include "Err.h"
#include "ErrorMessages.h"

Ref Real_OpAdd(Ref a, Ref b) {
    Obj * a_obj = GET_OBJ(a);
    if (a_obj == nullptr)
        return NEW_REF(new Err(ERR_FIRST_ARG_IS_NULL));
    assert(a_obj->Is(Real::t));

    Obj * b_obj = GET_OBJ(b);
    if (b_obj == nullptr)
        return NEW_REF(new Err(ERR_SECOND_ARG_IS_NULL));

    v_real a_val {((Real*)a_obj)->val };
    if (b_obj->Is(Real::t))
    {
        v_real b_val {((Real*)b_obj)->val };
        return NEW_REF(new Real(a_val + b_val));
    }
    else if (b_obj->Is(Int::t))
    {
        v_int b_val {((Int*)b_obj)->val };
        return NEW_REF(new Real(a_val + b_val));
    }
    return NEW_REF(new Err(ERR_OP_WRONG_TYPE));
}

Ref Real_OpSub(Ref a, Ref b) {
    Obj * a_obj = GET_OBJ(a);
    if (a_obj == nullptr)
        return NEW_REF(new Err(ERR_FIRST_ARG_IS_NULL));
    assert(a_obj->Is(Real::t));

    Obj * b_obj = GET_OBJ(b);
    if (b_obj == nullptr)
        return NEW_REF(new Err(ERR_SECOND_ARG_IS_NULL));

    v_real a_val {((Real*)a_obj)->val };
    if (b_obj->Is(Real::t))
    {
        v_real b_val {((Real*)b_obj)->val };
        return NEW_REF(new Real(a_val - b_val));
    }
    else if (b_obj->Is(Int::t))
    {
        v_int b_val {((Int*)b_obj)->val };
        return NEW_REF(new Real(a_val - b_val));
    }
    return NEW_REF(new Err(ERR_OP_WRONG_TYPE));
}

Ref Real_OpMult(Ref a, Ref b) {
    Obj * a_obj = GET_OBJ(a);
    if (a_obj == nullptr)
        return NEW_REF(new Err(ERR_FIRST_ARG_IS_NULL));
    assert(a_obj->Is(Real::t));

    Obj * b_obj = GET_OBJ(b);
    if (b_obj == nullptr)
        return NEW_REF(new Err(ERR_SECOND_ARG_IS_NULL));

    v_real a_val {((Real*)a_obj)->val };
    if (b_obj->Is(Real::t))
    {
        v_real b_val {((Real*)b_obj)->val };
        return NEW_REF(new Real(a_val * b_val));
    }
    else if (b_obj->Is(Int::t))
    {
        v_int b_val {((Int*)b_obj)->val };
        return NEW_REF(new Real(a_val * b_val));
    }
    return NEW_REF(new Err(ERR_OP_WRONG_TYPE));
}

Ref Real_OpDiv(Ref a, Ref b) {
    Obj * a_obj = GET_OBJ(a);
    if (a_obj == nullptr)
        return NEW_REF(new Err(ERR_FIRST_ARG_IS_NULL));
    assert(a_obj->Is(Real::t));

    Obj * b_obj = GET_OBJ(b);
    if (b_obj == nullptr)
        return NEW_REF(new Err(ERR_SECOND_ARG_IS_NULL));

    v_real a_val {((Real*)a_obj)->val };
    if (b_obj->Is(Real::t))
    {
        v_real b_val {((Real*)b_obj)->val };
        if (b_val == 0)
            return NEW_REF(new Err(ERR_DIVISION_BY_ZERO));
        return NEW_REF(new Real(a_val / b_val));
    }
    else if (b_obj->Is(Int::t))
    {
        v_int b_val {((Int*)b_obj)->val };
        if (b_val == 0)
            return NEW_REF(new Err(ERR_DIVISION_BY_ZERO));
        return NEW_REF(new Real(a_val / b_val));
    }
    return NEW_REF(new Err(ERR_OP_WRONG_TYPE));
}

Ref Real_OpPow(Ref a, Ref b) {
    Obj * a_obj = GET_OBJ(a);
    if (a_obj == nullptr)
        return NEW_REF(new Err(ERR_FIRST_ARG_IS_NULL));
    assert(a_obj->Is(Real::t));

    Obj * b_obj = GET_OBJ(b);
    if (b_obj == nullptr)
        return NEW_REF(new Err(ERR_SECOND_ARG_IS_NULL));

    v_real a_val {((Real*)a_obj)->val };
    if (b_obj->Is(Real::t)) {
        v_real b_val {((Real*)b_obj)->val };
        return NEW_REF(new Real(powl(a_val, b_val)));
    }
    else if (b_obj->Is(Int::t)) {
        v_int b_val {((Int*)b_obj)->val };
        return NEW_REF(new Real(powl(a_val, b_val)));
    }
    return NEW_REF(new Err(ERR_OP_WRONG_TYPE));
}

Ref Real_OpNeg(Ref a) {
    Obj * a_obj = GET_OBJ(a);
    if (a_obj == nullptr)
        return NEW_REF(new Err(ERR_FIRST_ARG_IS_NULL));
    assert(a_obj->Is(Real::t));
    v_real a_val {((Real*)a_obj)->val };
    return NEW_REF(new Real(-a_val));
}

Ref Real_OpGr(Ref a, Ref b) {
    Obj * a_obj = GET_OBJ(a);
    if (a_obj == nullptr)
        return NEW_REF(new Err(ERR_FIRST_ARG_IS_NULL));
    assert(a_obj->Is(Real::t));

    Obj * b_obj = GET_OBJ(b);
    if (b_obj == nullptr)
        return NEW_REF(new Err(ERR_SECOND_ARG_IS_NULL));

    v_real a_val {((Real*)a_obj)->val };
    if (b_obj->Is(Real::t)) {
        v_real b_val {((Real*)b_obj)-> val };
        return BOOL_REF(a_val > b_val);
    }
    else if (b_obj->Is(Int::t)) {
        v_int b_val {((Int*)b_obj)->val };
        return BOOL_REF(a_val > b_val);
    }
    return NEW_REF(new Err(ERR_OP_WRONG_TYPE));
}

Ref Real_OpGrEq(Ref a, Ref b) {
    Obj * a_obj = GET_OBJ(a);
    if (a_obj == nullptr)
        return NEW_REF(new Err(ERR_FIRST_ARG_IS_NULL));
    assert(a_obj->Is(Real::t));

    Obj * b_obj = GET_OBJ(b);
    if (b_obj == nullptr)
        return NEW_REF(new Err(ERR_SECOND_ARG_IS_NULL));

    v_real a_val {((Real*)a_obj)->val };
    if (b_obj->Is(Real::t)) {
        v_real b_val {((Real*)b_obj)-> val };
        return BOOL_REF(a_val >= b_val);
    }
    else if (b_obj->Is(Int::t)) {
        v_int b_val {((Int*)b_obj)->val };
        return BOOL_REF(a_val >= b_val);
    }
    return NEW_REF(new Err(ERR_OP_WRONG_TYPE));
}

Ref Real_OpLs(Ref a, Ref b) {
    Obj * a_obj = GET_OBJ(a);
    if (a_obj == nullptr)
        return NEW_REF(new Err(ERR_FIRST_ARG_IS_NULL));
    assert(a_obj->Is(Real::t));

    Obj * b_obj = GET_OBJ(b);
    if (b_obj == nullptr)
        return NEW_REF(new Err(ERR_SECOND_ARG_IS_NULL));

    v_real a_val {((Real*)a_obj)->val };
    if (b_obj->Is(Real::t)) {
        v_real b_val {((Real*)b_obj)-> val };
        return BOOL_REF(a_val < b_val);
    }
    else if (b_obj->Is(Int::t)) {
        v_int b_val {((Int*)b_obj)->val };
        return BOOL_REF(a_val < b_val);
    }
    return NEW_REF(new Err(ERR_OP_WRONG_TYPE));
}

Ref Real_OpLsEq(Ref a, Ref b) {
    Obj * a_obj = GET_OBJ(a);
    if (a_obj == nullptr)
        return NEW_REF(new Err(ERR_FIRST_ARG_IS_NULL));
    assert(a_obj->Is(Real::t));

    Obj * b_obj = GET_OBJ(b);
    if (b_obj == nullptr)
        return NEW_REF(new Err(ERR_SECOND_ARG_IS_NULL));

    v_real a_val {((Real*)a_obj)->val };
    if (b_obj->Is(Real::t)) {
        v_real b_val {((Real*)b_obj)-> val };
        return BOOL_REF(a_val <= b_val);
    }
    else if (b_obj->Is(Int::t)) {
        v_int b_val {((Int*)b_obj)->val };
        return BOOL_REF(a_val <= b_val);
    }
    return NEW_REF(new Err(ERR_OP_WRONG_TYPE));
}

Ref Real_OpEq(Ref a, Ref b) {
    if (a.index == b.index)
        return BOOL_REF(true);

    Obj * a_obj = GET_OBJ(a);
    if (a_obj == nullptr)
        return NEW_REF(new Err(ERR_FIRST_ARG_IS_NULL));
    assert(a_obj->Is(Real::t));

    Obj * b_obj = GET_OBJ(b);
    if (b_obj == nullptr)
        return NEW_REF(new Err(ERR_SECOND_ARG_IS_NULL));

    v_real a_val {((Real*)a_obj)->val };
    if (b_obj->Is(Real::t)) {
        v_real b_val {((Real*)b_obj)-> val };
        return BOOL_REF(a_val == b_val);
    }
    else if (b_obj->Is(Int::t)) {
        v_int b_val {((Int*)b_obj)->val };
        return BOOL_REF(a_val == b_val);
    }
    return NEW_REF(new Err(ERR_OP_WRONG_TYPE));
}

Ref Real_OpNotEq(Ref a, Ref b) {
    if (a.index == b.index)
        return BOOL_REF(false);

    Obj * a_obj = GET_OBJ(a);
    if (a_obj == nullptr)
        return NEW_REF(new Err(ERR_FIRST_ARG_IS_NULL));
    assert(a_obj->Is(Real::t));

    Obj * b_obj = GET_OBJ(b);
    if (b_obj == nullptr)
        return NEW_REF(new Err(ERR_SECOND_ARG_IS_NULL));

    v_real a_val {((Real*)a_obj)->val };
    if (b_obj->Is(Real::t)) {
        v_real b_val {((Real*)b_obj)-> val };
        return BOOL_REF(a_val != b_val);
    }
    else if (b_obj->Is(Int::t)) {
        v_int b_val {((Int*)b_obj)->val };
        return BOOL_REF(a_val != b_val);
    }
    return NEW_REF(new Err(ERR_OP_WRONG_TYPE));
}

///////////////////////////////////////////////////////////////////////////////

Type * Real::t;

void Real::InitType() {
    Real::t = new Type("real");
    auto mt = t->opTable;
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

Real::Real() : Obj{Real::t} {}

Real::Real(v_real value) : Obj{Real::t}, val{value} {}

std::string Real::ToStr() const {
    std::stringstream s;
    s << val;
    return s.str();
}

