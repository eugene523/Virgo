#include <cassert>
#include <sstream>
#include "Type.h"
#include "Bool.h"
#include "Error.h"
#include "Str.h"

const char * ERR_LOGICAL_EXPR_WRONG_TYPE = "Logical expressions can only be performed on objects of a boolean type.";

Obj * Bool_OpAnd(Obj * self, Obj * other) {
    assert(self->Is(Bool::t));

    if (!(other->Is(Bool::t)))
        return (Obj*)Error::New(ERR_LOGICAL_EXPR_WRONG_TYPE);

    bool selfVal = ((Bool*)self)->val;
    bool otherVal = ((Bool*)other)->val;
    return (Obj*)Bool::New(selfVal && otherVal);
}

Obj * Bool_OpOr(Obj * self, Obj * other) {
    assert(self->Is(Bool::t));

    if (!(other->Is(Bool::t)))
        return (Obj*)Error::New(ERR_LOGICAL_EXPR_WRONG_TYPE);

    bool selfVal = ((Bool*)self)->val;
    bool otherVal = ((Bool*)other)->val;
    return (Obj*)Bool::New(selfVal || otherVal);
}

Obj * Bool_OpNot(Obj * self) {
    assert(self->Is(Bool::t));
    bool selfVal = ((Bool*)self)->val;
    return (Obj*)Bool::New(selfVal);
}

Obj * Bool_OpEq(Obj * self, Obj * other) {
    assert(self->Is(Bool::t));

    if (!(other->Is(Bool::t)))
        return (Obj*)Bool::New(false);

    bool selfVal = ((Bool*)self)->val;
    bool otherVal = ((Bool*)other)->val;
    return (Obj*)Bool::New(selfVal == otherVal);
}

Obj * Bool_OpNotEq(Obj * self, Obj * other) {
    assert(self->Is(Bool::t));

    if (!(other->Is(Bool::t)))
        return (Obj*)Bool::New(true);

    bool selfVal = ((Bool*)self)->val;
    bool otherVal = ((Bool*)other)->val;
    return (Obj*)Bool::New(selfVal != otherVal);
}

///////////////////////////////////////////////////////////////////////////////

Type * Bool::t;

Bool * Bool::True {};

Bool * Bool::False {};

void Bool::InitType() {
    Bool::t = new Type("bool");
    auto mtab = t->methodTable;
    mtab->OpAnd   = &Bool_OpAnd;
    mtab->OpOr    = &Bool_OpOr;
    mtab->OpNot   = &Bool_OpNot;
    mtab->OpEq    = &Bool_OpEq;
    mtab->OpNotEq = &Bool_OpNotEq;

    Bool::True = (Bool*)Heap::GetChunk_Constant(sizeof(Bool));
    Bool::True->val = true;

    Bool::False = (Bool*)Heap::GetChunk_Constant(sizeof(Bool));
    Bool::False->val = false;
}