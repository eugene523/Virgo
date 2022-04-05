#include <cassert>
#include <utility>
#include "Type.h"
#include "Str.h"
#include "Bool.h"
#include "Int.h"
#include "Args.h"
#include "Err.h"
#include "ErrorMessages.h"

Ref Str_OpAdd(Ref a, Ref b) {
    Obj * a_obj = GET_OBJ(a);
    if (a_obj == nullptr)
        return NEW_REF(new Err(ERR_FIRST_ARG_IS_NULL));
    assert(a_obj->Is(Str::t));

    Obj * b_obj = GET_OBJ(b);
    if (b_obj == nullptr)
        return NEW_REF(new Err(ERR_SECOND_ARG_IS_NULL));

    std::string a_val { ((Str*)a_obj)->val };
    if (b_obj->Is(Str::t)) {
        std::string b_val { ((Str*)b_obj)->val };
        return NEW_REF(new Str(a_val + b_val));
    }
    return NEW_REF(new Err(ERR_OP_WRONG_TYPE));
}

Ref Str_OpEq(Ref a, Ref b) {
    Obj * a_obj = GET_OBJ(a);
    if (a_obj == nullptr)
        return NEW_REF(new Err(ERR_FIRST_ARG_IS_NULL));
    assert(a_obj->Is(Str::t));

    Obj * b_obj = GET_OBJ(b);
    if (b_obj == nullptr)
        return NEW_REF(new Err(ERR_SECOND_ARG_IS_NULL));

    std::string a_val { ((Str*)a_obj)->val };
    if (b_obj->Is(Str::t)) {
        std::string b_val { ((Str*)b_obj)->val };
        return BOOL_REF(a_val == b_val);
    }
    return NEW_REF(new Err(ERR_OP_WRONG_TYPE));
}

Ref Str_OpNotEq(Ref a, Ref b) {
    Obj * a_obj = GET_OBJ(a);
    if (a_obj == nullptr)
        return NEW_REF(new Err(ERR_FIRST_ARG_IS_NULL));
    assert(a_obj->Is(Str::t));

    Obj * b_obj = GET_OBJ(b);
    if (b_obj == nullptr)
        return NEW_REF(new Err(ERR_SECOND_ARG_IS_NULL));

    std::string a_val { ((Str*)a_obj)->val };
    if (b_obj->Is(Str::t)) {
        std::string b_val { ((Str*)b_obj)->val };
        return BOOL_REF(a_val != b_val);
    }
    return NEW_REF(new Err(ERR_OP_WRONG_TYPE));
}

Ref Str_Get(Ref a, Ref b) {
    Obj * a_obj = GET_OBJ(a);
    if (a_obj == nullptr)
        return NEW_REF(new Err(ERR_FIRST_ARG_IS_NULL));
    assert(a_obj->Is(Str::t));
    Str * str = (Str*)a_obj;

    Obj * b_obj = GET_OBJ(b);
    if (b_obj == nullptr)
        return NEW_REF(new Err(ERR_SECOND_ARG_IS_NULL));
    assert(b_obj->Is(Args::t));
    auto * argsObj = (Args*)b_obj;
    Ref indexRef = argsObj->Get(0);
    Obj * indexObj = GET_OBJ(indexRef);
    if (indexObj->Is(Int::t)) {
        Int * index = (Int*)indexObj;
        if (index->val < 0 || index->val >= str->val.length())
            return NEW_REF(new Err("Index is out of range."));
        return NEW_REF(new Str(str->val[index->val]));
    }
    return NEW_REF(new Err("Index must be a positive integer."));
}

///////////////////////////////////////////////////////////////////////////////

Type * Str::t;

void Str::InitType() {
    Str::t = new Type("str");
    auto ot = Str::t->opTable;
    ot->OpAdd   = &Str_OpAdd;
    ot->OpEq    = &Str_OpEq;
    ot->OpNotEq = &Str_OpNotEq;
    ot->Get     = &Str_Get;
}

Str::Str(std::string val) : Obj{Str::t}, val{std::move(val)} {}

Str::Str(char val) : Obj{Str::t}, val{val} {}

std::string Str::ToStr() const {
    return val;
}
