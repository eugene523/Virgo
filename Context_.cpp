#include <cassert>
#include <iostream>
#include <sstream>
#include "Context_.h"
#include "Type.h"
#include "Str.h"
#include "Fun.h"
#include "Script_.h"
#include "Error.h"
#include "Builtins.h"

Context::Context() = default;

Ref Context::GetVariable(Ref name) {
    Obj * nameStr = GET_OBJ(name);
    assert(nameStr->Is(Str::t));
    if (variables.count(name) != 0)
        return variables[name];
    assert(executionOwner != Ref::none);
    Obj * execOwnerObj = GET_OBJ(executionOwner);
    if (execOwnerObj->Is(Fun::t)) {
        auto * fun = (Fun*)execOwnerObj;
        Ref childRef = fun->GetChildDef(name);
        if (childRef != Ref::none)
            return childRef;
        auto * parent = GET_OBJ(fun->GetParentDef());
        assert(parent->Is(Script::t));
        childRef = ((Script*) parent)->GetChildDef(name);
        if (childRef != Ref::none)
            return childRef;
        return Builtins::ZeroNamespace::Get(name);
    } else if (execOwnerObj->Is(Script::t)) {
        auto * script = (Script*)execOwnerObj;
        Ref childRef = script->GetChildDef(name);
        if (childRef != Ref::none)
            return childRef;
        return Builtins::ZeroNamespace::Get(name);
    }
    std::stringstream s;
    s << "No such name '" << ((Str*)nameStr)->ToStr() << "' exists in the current context.";
    return NEW_REF(new Err(s.str()));
}

void Context::SetVariable(Ref name, Ref val) {
    assert(GET_OBJ(name)->Is(Str::t));
    assert(!(GET_OBJ(val)->Is(Err::t)));
    if (variables.count(name) != 0)
        variables.erase(name);
    variables.emplace(name, val.index);
}

void Context::Mark() {
    for (auto const & [key, val] : variables)
        Mem::MarkRef(val);
}

void Context::Print() {
    std::cout << "Number of names: " << variables.size() << std::endl;
    for (auto const & [key, val] : variables) {
        std::cout << GET_OBJ(key)->ToStr() << " = " << GET_OBJ(val)->ToStr() << std::endl;
    }
}
