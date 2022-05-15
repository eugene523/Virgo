#include <cassert>
#include "Object.h"
#include "Type.h"
#include "Error.h"

Ref Object_GetField(Ref selfRef, Ref fieldName) {
    auto * selfObj = GET_OBJ(selfRef);
    assert(selfObj->Is(Object::t));
    auto * self = (Object*)selfObj;
    if (self->fields.count(fieldName) != 0)
        return self->fields[fieldName];
    return NEW_REF(new Err("No such field"));
}

Ref Object_SetField(Ref selfRef, Ref fieldName, Ref val) {
    auto * selfObj = GET_OBJ(selfRef);
    assert(selfObj->Is(Object::t));
    auto * self = (Object*)selfObj;
    self->fields[fieldName] = val;
    return Ref::none;
}

Type * Object::t;

void Object::InitType() {
    Object::t = new Type("object");
    auto ot = Object::t->opTable;
    ot->GetField = &Object_GetField;
    ot->SetField = &Object_SetField;
}

void Object::Mark() {

}

Object::Object() : Obj{Object::t} {}
