#include <sstream>
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

void Context::SetVariable(Obj * name, Obj * value) {
    assert(name->Is(Str::t));
    assert(value != nullptr);
    variables[name] = value;
}