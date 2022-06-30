#include <cassert>
#include <utility>
#include <string>
#include "Type.h"
#include "Str.h"
#include "Bool.h"
#include "Int.h"
//#include "Args.h"
#include "Error.h"
#include "ErrorMessages.h"

const char * str_concat(const char * a, const char * b) {
    uint aLen = strlen(a);
    uint bLen = strlen(b);
    uint cTotalLen = aLen + bLen + 1;
    char * c = (char*)calloc(cTotalLen, sizeof(char));
    memcpy(c, a, aLen);
    memcpy(c + aLen, b, bLen);
    return c;
}

Obj * Str_OpAdd(Obj * self, Obj * other) {
    assert(self->Is(Str::t));
    const char * selfVal = ((Str*)self)->val;
    if (other->Is(Str::t))
    {
        const char * otherVal = ((Str*)other)->val;
        const char * concatenated = str_concat(selfVal, otherVal);
        return (Obj*)Str::New(concatenated);
    }
    return (Obj*)Error::New(ERROR_INCOMPATIBLE_TYPES);
}

Obj * Str_OpEq(Obj * self, Obj * other) {
    assert(self->Is(Str::t));
    if (self == other)
        return (Obj*)Bool::True;

    const char * selfVal = ((Str*)self)->val;
    if (other->Is(Str::t))
    {
        const char * otherVal = ((Str*)other)->val;
        return (Obj*)Bool::New(strcmp(selfVal, otherVal));
    }
    return (Obj*)Error::New(ERROR_INCOMPATIBLE_TYPES);
}

Obj * Str_OpNotEq(Obj * self, Obj * other) {
    Obj * result = Str_OpEq(self, other);
    if (result->Is(Error::t))
        return result;
    return (Obj*)(((Bool*)result)->Invert());
}

std::string Str_Dstr(Obj * self) {
    assert(self->Is(Str::t));
    auto * str = (Str*)self;
    return std::string(str->val);
}

/*
Obj * Str_Get(Obj * self, Obj * other) {
    assert(self->Is(Str::t));
    Str * str = (Str*)self;

    Obj * other = GET_OBJ(other);
    if (other == nullptr)
        return NEW_REF(new Error(ERR_SECOND_ARG_IS_NULL));
    assert(other->Is(Args::t));
    auto * argsObj = (Args*)other;
    Obj * indexRef = argsObj->Get(0);
    Obj * indexObj = GET_OBJ(indexRef);
    if (indexObj->Is(Int::t)) {
        Int * index = (Int*)indexObj;
        if (index->val < 0 || index->val >= str->val.length())
            return NEW_REF(new Error("Index is out of range."));
        return (Obj*)Str::New(str->val[index->val]));
    }
    return NEW_REF(new Error("Index must be self positive integer."));
}
 */

///////////////////////////////////////////////////////////////////////////////

Type * Str::t;

void Str::InitType() {
    Str::t = new Type("str");
    auto mt = Str::t->methodTable;
    mt->OpAdd   = &Str_OpAdd;
    mt->OpEq    = &Str_OpEq;
    mt->OpNotEq = &Str_OpNotEq;
    mt->Dstr    = &Str_Dstr;
    //mt->Get     = &Str_Get;
}

void Str::New(void * inPlace, const char * value) {
    Str * s = (Str*)inPlace;
    Obj::Init(s, Str::t);
    uint len = strlen(value);
    char * s_val = (char*)calloc(len + 1, sizeof(char));
    memcpy(s_val, value, len);
    s->val = s_val;
    s->len = len;
}

Str * Str::New(const char * value) {
    Str * s = (Str*)Heap::GetChunk_Baby(sizeof(Str));
    New(s, value);
    return s;
}
