#include <cassert>
#include <sstream>
#include "Type.h"
#include "Bool.h"
#include "Error.h"
#include "Str.h"

const char * ERROR_LOGICAL_EXPR_WRONG_TYPE =
"Logical expressions can only be performed on objects of a boolean type.";

void Bool_Equal(Obj * self, Obj * other) {
    assert(self->Is(Bool::t));

    if (!(other->Is(Bool::t)))
        return false;

    bool selfVal  = ((Bool*)self)->val;
    bool otherVal = ((Bool*)other)->val;
    return selfVal == otherVal;
}

Obj * Bool::Not(Obj * self) {
    assert(self->Is(Bool::t));
    bool selfVal = ((Bool*)self)->val;
    return (Obj*)Bool::New(!selfVal);
}

Obj * Bool::And(Obj * self, Obj * other) {
    assert(self->Is(Bool::t));

    if (!(other->Is(Bool::t)))
        return (Obj*)Error::New(ERROR_LOGICAL_EXPR_WRONG_TYPE);

    bool selfVal = ((Bool*)self)->val;
    bool otherVal = ((Bool*)other)->val;
    return (Obj*)Bool::New(selfVal && otherVal);
}

Obj * Bool::Or(Obj * self, Obj * other) {
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

Bool::Bool(bool value) : Obj{Bool::t}, value{value} {}

void Bool::InitType() {
    Bool::t      = new Type("bool");
    auto mt      = t->methodTable;
    mt->DebugStr = &Bool_DebugStr;
}

void Bool::InitConstants() {
    std::byte * inPlace = Heap::GetChunk_Constant(sizeof(Bool));
    Bool::True = new (inPlace) Bool(true);

    inPlace = Heap::GetChunk_Constant(sizeof(Bool));
    Bool::False = new (inPlace) Bool(false);
}
