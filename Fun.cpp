#include <cassert>
#include <sstream>
#include <algorithm>
#include "Fun.h"
#include "Type.h"
#include "Err.h"
#include "Str.h"
#include "ArgPair.h"
#include "Args.h"
#include "Return.h"
#include "VM.h"

Type * Fun::t;

void Fun::InitType() {
    Fun::t = new Type("fun");
}

Fun::Fun(Ref name) : Obj{Fun::t}, name{name} {}

Fun::~Fun() = default;

Ref Fun::GetName() { return name; }

void Fun::Mark() {}

void Fun::SetSelf(Ref selfRef) { self = selfRef; }

void Fun::AddArgument(Ref argName, Ref argDefaultValue /* = Ref::none */) {
    argNames.push_back(argName);
    argDefaultValues.push_back(argDefaultValue);
    argPositions.emplace(argName, numOfArgs);
    if (argDefaultValue == Ref::none)
        numOfRequiredArgs++;
    numOfArgs++;
}

void Fun::AddExpression(Expr * expr) {
    expressions.push_back(expr);
}

void Fun::SetParentDef(Ref parent) {
    parentDefinition = parent;
}

Ref Fun::GetParentDef() {
    return parentDefinition;
}

void Fun::AddChildDef(Ref childName, Ref child) {
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
}

Ref Fun::GetChildDef(Ref childName) {
    return childDefinitions[childName];
}

void Fun::PrepareContext() {
    auto ctx = new Context();
    ctx->executionOwner = self;
    VM::contextStack.Push(ctx);
}

Ref Fun::ValidateArguments(Ref argList) {
    if (argList == Ref::none) {
        if (numOfRequiredArgs > 0)
            return Err_NotEnoughArgs(0);
    } else {
        auto * argListObj = (Args*)GET_OBJ(argList);
        assert(argListObj->Is(Args::t));
        int numOfPassedArgs = argListObj->NumOfArguments();

        if (numOfPassedArgs < numOfRequiredArgs)
            return Err_NotEnoughArgs(numOfPassedArgs);

        if (numOfPassedArgs > numOfArgs)
            return Err_TooManyArgs(numOfPassedArgs);
    }
    return Ref::none;
}

Ref Fun::PrepareArguments(Ref argsRef) {
    if (numOfArgs != 0) {
        std::vector<bool> settedArgs;
        settedArgs.resize(numOfArgs, false);

        auto argsListObj = (Args*)GET_OBJ(argsRef);
        uint numOfPassedArgs = argsListObj->NumOfArguments();

        for (uint i = 0; i < numOfPassedArgs; i++) {
            auto argRef = (argsListObj->Get(i));
            auto argObj = GET_OBJ(argRef);
            if (argObj->Is(ArgPair::t)) {
                auto pair = (ArgPair*)argObj;
                if (std::find(argNames.begin(), argNames.end(), pair->name) == argNames.end())
                    return Err_NoSuchArg(pair->name);
                VM::contextStack.Last()->SetVariable(pair->name, pair->val);
                settedArgs[argPositions[pair->name]] = true;
            } else {
                auto argName = argNames[i];
                VM::contextStack.Last()->SetVariable(argName, argRef);
                settedArgs[argPositions[argName]] = true;
            }
        }

        for (uint i = 0; i < numOfArgs; i++) {
            if (!settedArgs[i]) {
                return Err_ArgIsNotSet(argNames[i]);
            }
        }
    }
    return Ref::none;
}

Ref Fun::PerformExecution() {
    for (auto e : expressions) {
        Ref result = e->Execute();
        if (result == Ref::none)
            continue;
        
        auto * resultObj = GET_OBJ(result);
        
        if (resultObj->Is(Return::t))
            return ((Return*)resultObj)->retRef;

        if (resultObj->Is(Err::t))
            return result;
    }
    return Ref::none;
}

void Fun::PostExecution() {
    VM::contextStack.Pop();
}

Ref Fun::Execute(Ref argList) {
    PrepareContext();

    Ref validationResult = ValidateArguments(argList);
    if (validationResult != Ref::none)
        return validationResult;

    Ref preparationResult = PrepareArguments(argList);
    if (preparationResult != Ref::none)
        return preparationResult;

    Ref executionResult = PerformExecution();
    PostExecution();
    return executionResult;
}

#define REF_TO_STD_STRING(ref) ((Str*)GET_OBJ(ref))->val

Ref Fun::Err_NotEnoughArgs(uint numOfPassedArgs) {
    std::stringstream s;
    s << "Not enough arguments. Function '"
      << REF_TO_STD_STRING(name) << "' requires " << numOfRequiredArgs
      << " arguments, but " << numOfPassedArgs << " was passed.";
    return NEW_REF(new Err(s.str()));
}

Ref Fun::Err_NoSuchArg(Ref passedArgName) {
    std::stringstream s;
    s << "No such argument '"
      << REF_TO_STD_STRING(passedArgName)
      << "' in function '" << REF_TO_STD_STRING(name) << "' declaration.";
    return NEW_REF(new Err(s.str()));
}

Ref Fun::Err_TooManyArgs(uint numOfPassedArgs) {
   std::stringstream s;
   s << "Too many arguments. Function '"
     << REF_TO_STD_STRING(name) << "' requires " << numOfRequiredArgs
     << " arguments, but " << numOfPassedArgs << " was passed.";
   return NEW_REF(new Err(s.str()));
}

Ref Fun::Err_ArgIsNotSet(Ref argName) {
    std::stringstream s;
    s << "In function '" << REF_TO_STD_STRING(name)
      << "' argument '" << REF_TO_STD_STRING(argName) << "' is not setted.";
    return NEW_REF(new Err(s.str()));
}
