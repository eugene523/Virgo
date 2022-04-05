#include "Class.h"
#include "Str.h"
#include "Type.h"
#include "Object.h"

Type * Class::t;

void Class::InitType() {
    Class::t = new Type("class");
}

void Class::SetSelf(Ref selfRef) {
    self = selfRef;
}

void Class::SetParentDef(Ref parent) {
    parentDefinition = parent;
}

Ref Class::GetParentDef() {
    return parentDefinition;
}

void Class::AddChildDef(Ref childName, Ref child) {
    /*
    auto * childFunc = (Fun*)GET_OBJ(child);
    assert(childFunc->Is(Fun::t));
    childFunc->parentDefinition = self;

    if (childDefinitions.count(childName) != 0) {
        std::cerr << "Duplicate definition of "
                  << ((Str*)GET_OBJ(childName))->val
                  << " in "
                  << ((Str*)GET_OBJ(name))->val
                  << " function.";
        abort();
    }
    childDefinitions.emplace(childName, child);
    */
}

Ref Class::GetChildDef(Ref childName) {
    return childDefinitions[childName];
}

Class::Class(Ref name) : Obj{Class::t}, name{name} {}

void Class::AddField(Ref fieldName) {
    fieldNames.push_back(fieldName);
}

void Class::Mark() {}

Ref Class::CreateInstance(Ref args) {
    auto * object = new Object();
    for (auto f : fieldNames) {
        object->fields[f] = Ref::none;
    }
    return NEW_REF(object);
}
