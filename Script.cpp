#include <cassert>
#include "Script.h"
#include "Type.h"
#include "Expr.h"
#include "Fun.h"
#include "VM.h"
#include "Str.h"
#include "Err.h"

Type * Script::t;

void Script::InitType() {
    Script::t = new Type("script");
}

Script::Script() : Obj {Script::t} {}

Script::~Script() = default;

void Script::AddExpr(Expr * expr) {
    expressions.push_back(expr);
}

void Script::SetParentDefinition(Ref parent) {
    parentDefinition = parent;
}

Ref Script::GetParentDefinition() {
    return parentDefinition;
}

void Script::AddChildDefinition(Ref childName, Ref child) {
    auto * childObj = GET_OBJ(child);
    assert(childObj->Is(Fun::t));
    Fun * childFun = (Fun*)childObj;
    childFun->SetParentDefinition(self);

    if (childDefinitions.count(childName) != 0) {
        std::cerr << "Duplicate definition of " << ((Str*)GET_OBJ(childName))->val << " in script.";
        abort();
    }
    childDefinitions.emplace(childName, child);
}

Ref Script::GetChildDefinition(Ref childName) {
    return childDefinitions[childName];
}

void Script::PreExecution() {
    auto * context = VM::contextStack.Create();
    context->executionOwner = self;
}

void Script::PostExecution() {
    VM::contextStack.Pop();
}

Ref Script::Execute() {
    PreExecution();
    for (auto e : expressions) {
        Ref result = e->Execute();
        if (result != Ref::none && GET_OBJ(result)->Is(Err::t)) {
            PostExecution();
            return result;
        }
    }
    VM::contextStack.Print();
    PostExecution();
    return Ref::none;
}

void Script::Mark() {}