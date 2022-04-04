#include "Class.h"
#include "Str.h"
#include "Type.h"

Type * Class::t;

void Class::InitType() {
    Class::t = new Type("class");
}

void Class::SetParentDefinition(Ref parent) {
    parentDefinition = parent;
}

Ref Class::GetParentDefinition() {
    return parentDefinition;
}

void Class::AddChildDefinition(Ref childName, Ref child) {
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

Ref Class::GetChildDefinition(Ref childName) {
    return childDefinitions[childName];
}

Class::Class(Ref name) : Obj{Class::t}, name{name} {}

void Class::AddField(Ref fieldName) {
    fieldNames.push_back(fieldName);
}

void Class::Mark() {}
