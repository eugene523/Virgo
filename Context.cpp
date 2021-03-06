#include <sstream>
#include <iostream>
#include "Context.h"
#include "Str.h"
#include "Error.h"

Obj * Context::GetVariable(Obj * name) {
    assert(name->Is(Str::t));
    if (variables.count(name) != 0)
        return variables[name];

    std::stringstream s;
    s << "No such name '"
      << ((Str*)name)->val
      << "' exists in the current context.";
    return (Obj*)Error::New(s.str());
}

Obj * Context::SetVariable(Obj * name, Obj * value) {
    assert(name->Is(Str::t));
    assert(value != nullptr);
    variables[name] = value;
    return nullptr;
}

void Context::Print() {
    for (auto const & [key, val] : variables) {
        std::string keyStr, valStr;
        keyStr = key->type->methodTable->DebugStr(key);
        auto * method = val->type->methodTable->DebugStr;
        if (method == nullptr) {
            valStr = val->type->name;
        } else {
            valStr = method(val);
        }
        std::cout << '\n' << keyStr << " = " << valStr;
    }
}
