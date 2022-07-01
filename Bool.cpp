#include <cassert>
#include <sstream>
#include "Type.h"
#include "Bool.h"
#include "Error.h"
#include "Str.h"

const char * ERROR_LOGICAL_EXPR_WRONG_TYPE = "Logical expressions can only be performed on objects of a boolean type.";

Obj * Bool_Eq(Obj * self, Obj * other) {
    assert(self->Is(Bool::t));

    if (!(other->Is(Bool::t)))
        return (Obj*)Bool::New(false);

    bool selfVal = ((Bool*)self)->val;
    bool otherVal = ((Bool*)other)->val;
    return (Obj*)Bool::New(selfVal == otherVal);
}

Obj * Bool_NotEq(Obj * self, Obj * other) {
    assert(self->Is(Bool::t));

    if (!(other->Is(Bool::t)))
        return (Obj*)Bool::New(true);

    bool selfVal = ((Bool*)self)->val;
    bool otherVal = ((Bool*)other)->val;
    return (Obj*)Bool::New(selfVal != otherVal);
}

Obj * Bool_Not(Obj * self) {
    assert(self->Is(Bool::t));
    bool selfVal = ((Bool*)self)->val;
    return (Obj*)Bool::New(!selfVal);
}

Obj * Bool_And(Obj * self, Obj * other) {
    assert(self->Is(Bool::t));

    if (!(other->Is(Bool::t)))
        return (Obj*)Error::New(ERROR_LOGICAL_EXPR_WRONG_TYPE);

    bool selfVal = ((Bool*)self)->val;
    bool otherVal = ((Bool*)other)->val;
    return (Obj*)Bool::New(selfVal && otherVal);
}

Obj * Bool_Or(Obj * self, Obj * other) {
    assert(self->Is(Bool::t));

    if (!(other->Is(Bool::t)))
        return (Obj*)Error::New(ERROR_LOGICAL_EXPR_WRONG_TYPE);

    bool selfVal = ((Bool*)self)->val;
    bool otherVal = ((Bool*)other)->val;
    return (Obj*)Bool::New(selfVal || otherVal);
}

std::string Bool_DebugStr(Obj * self) {
    assert(self->type == Bool::t);
    return (self == (Obj*)Bool::True) ? "true" : "false";
}

///////////////////////////////////////////////////////////////////////////////

Type * Bool::t;

Bool * Bool::True;

Bool * Bool::False;

void Bool::InitType() {
    Bool::t = new Type("bool");
    auto mt = t->methodTable;
    mt->Eq       = &Bool_Eq;
    mt->NotEq    = &Bool_NotEq;
    mt->Not      = &Bool_Not;
    mt->And      = &Bool_And;
    mt->Or       = &Bool_Or;
    mt->DebugStr = &Bool_DebugStr;

    Bool::True = (Bool*)Heap::GetChunk_Constant(sizeof(Bool));
    Obj::Init(Bool::True, Bool::t);
    Bool::True->val = true;

    Bool::False = (Bool*)Heap::GetChunk_Constant(sizeof(Bool));
    Obj::Init(Bool::False, Bool::t);
    Bool::False->val = false;
}
