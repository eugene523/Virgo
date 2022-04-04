#include <cassert>
#include <sstream>
#include "Type.h"
#include "Bool.h"
#include "Err.h"
#include "ErrorMessages.h"
#include "Str.h"

const char * ERROR_LOG_EXPR = "Logical expressions can only be performed on objects of a boolean type.";

Ref Bool_OpAnd(Ref a, Ref b) {
    Obj * a_obj = GET_OBJ(a);
    if (a_obj == nullptr)
        return NEW_REF(new Err(ERR_FIRST_ARG_IS_NULL));
    assert(a_obj->Is(Bool::t));

    Obj * b_obj = GET_OBJ(b);
    if (b_obj == nullptr)
        return NEW_REF(new Err(ERR_SECOND_ARG_IS_NULL));

    if (!(b_obj->Is(Bool::t)))
        return NEW_REF(new Err(ERROR_LOG_EXPR));

    bool a_val = ((Bool*)a_obj)->val;
    bool b_val = ((Bool*)b_obj)->val;
    return BOOL_REF(a_val && b_val);
}

Ref Bool_OpOr(Ref a, Ref b) {
    Obj * a_obj = GET_OBJ(a);
    if (a_obj == nullptr)
        return NEW_REF(new Err(ERR_FIRST_ARG_IS_NULL));
    assert(a_obj->Is(Bool::t));

    Obj * b_obj = GET_OBJ(b);
    if (b_obj == nullptr)
        return NEW_REF(new Err(ERR_SECOND_ARG_IS_NULL));

    if (!(b_obj->Is(Bool::t)))
        return NEW_REF(new Err(ERROR_LOG_EXPR));

    bool a_val = ((Bool*)a_obj)->val;
    bool b_val = ((Bool*)b_obj)->val;
    return BOOL_REF(a_val || b_val);
}

Ref Bool_OpXor(Ref a, Ref b) {
    Obj * a_obj = GET_OBJ(a);
    if (a_obj == nullptr)
        return NEW_REF(new Err(ERR_FIRST_ARG_IS_NULL));
    assert(a_obj->Is(Bool::t));

    Obj * b_obj = GET_OBJ(b);
    if (b_obj == nullptr)
        return NEW_REF(new Err(ERR_SECOND_ARG_IS_NULL));

    if (!(b_obj->Is(Bool::t)))
        return NEW_REF(new Err(ERROR_LOG_EXPR));

    bool a_val = ((Bool*)a_obj)->val;
    bool b_val = ((Bool*)b_obj)->val;
    return BOOL_REF(a_val xor b_val);
}

Ref Bool_OpNot(Ref a) {
    Obj * a_obj = GET_OBJ(a);
    if (a_obj == nullptr)
        return NEW_REF(new Err(ERR_FIRST_ARG_IS_NULL));
    assert(a_obj->Is(Bool::t));

    bool a_val = ((Bool*)a_obj)->val;
    return BOOL_REF(!a_val);
}

Ref Bool_OpEq(Ref a, Ref b) {
    Obj * a_obj = GET_OBJ(a);
    if (a_obj == nullptr)
        return NEW_REF(new Err(ERR_FIRST_ARG_IS_NULL));
    assert(a_obj->Is(Bool::t));

    Obj * b_obj = GET_OBJ(b);
    if (b_obj == nullptr)
        return NEW_REF(new Err(ERR_SECOND_ARG_IS_NULL));

    if (!(b_obj->Is(Bool::t)))
        return BOOL_REF(false);

    bool a_val = ((Bool*)a_obj)->val;
    bool b_val = ((Bool*)b_obj)->val;
    return BOOL_REF(a_val == b_val);
}

Ref Bool_OpNotEq(Ref a, Ref b) {
    Obj * a_obj = GET_OBJ(a);
    if (a_obj == nullptr)
        return NEW_REF(new Err(ERR_FIRST_ARG_IS_NULL));
    assert(a_obj->Is(Bool::t));

    Obj * b_obj = GET_OBJ(b);
    if (b_obj == nullptr)
        return NEW_REF(new Err(ERR_SECOND_ARG_IS_NULL));

    if (!(b_obj->Is(Bool::t)))
        return BOOL_REF(true);

    bool a_val = ((Bool*)a_obj)->val;
    bool b_val = ((Bool*)b_obj)->val;
    return BOOL_REF(a_val != b_val);
}

Ref Bool_ToStr(Ref a) {
    Obj * a_obj = GET_OBJ(a);
    if (a_obj == nullptr)
        return NEW_REF(new Err(ERR_FIRST_ARG_IS_NULL));
    assert(a_obj->Is(Bool::t));
    auto val = ((Bool*)a_obj)->val;

    std::stringstream s;
    s << (val ? "true" : "false");
    return NEW_REF(new Str(s.str()));
}

///////////////////////////////////////////////////////////////////////////////

Type * Bool::t;

void Bool::InitType() {
    Bool::t = new Type("bool");
    auto ot = t->opTable;
    ot->OpAnd   = &Bool_OpAnd;
    ot->OpOr    = &Bool_OpOr;
    ot->OpXor   = &Bool_OpXor;
    ot->OpNot   = &Bool_OpNot;
    ot->OpEq    = &Bool_OpEq;
    ot->OpNotEq = &Bool_OpNotEq;
    ot->ToStr   = &Bool_ToStr;
}

Ref Bool::True {};

Ref Bool::False {};

Bool::Bool() : Obj{Bool::t} {}

Bool::Bool(bool val) : Obj{Bool::t}, val{val} {}

Bool::~Bool() = default;

std::string Bool::ToStr() const {
    std::stringstream s;
    s << (val ? "true" : "false");
    return s.str();
}
