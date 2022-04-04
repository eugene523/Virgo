#include <cassert>
#include <sstream>
#include "Builtins.h"
#include "VM.h"
#include "Type.h"
#include "ArgList.h"
#include "Err.h"
#include "Bool.h"
#include "Int.h"
#include "Real.h"
#include "Str.h"
#include "List.h"
#include "Seg.h"

namespace Builtins {

Assert::Assert() : NFun{"assert"} {}

Ref Assert::Execute(Ref args) {
    auto argListObj = GET_OBJ(args);
    assert(argListObj != nullptr);
    assert(argListObj->Is(ArgList::t));
    auto argList = (ArgList*)argListObj;
    assert(argList->NumOfArguments() == 2);

    auto intObj = GET_OBJ(argList->Get(1));
    assert(intObj != nullptr);
    assert(intObj->Is(Int::t));
    v_int line = ((Int*)intObj)->val;

    auto boolObj = GET_OBJ(argList->Get(0));
    assert(boolObj != nullptr);
    if (!boolObj->Is(Bool::t))
        return NEW_REF(new Err("Argument of an assert function must be a logical expression.", line));
    bool assertResult = ((Bool*)boolObj)->val;

    if (!assertResult)
        return NEW_REF(new Err("Assertion failed.", line));
    return Ref::none;
}

///////////////////////////////////////////////////////////////////////////////

Print::Print() : NFun{"print"} {}

Ref Print::Execute(Ref args) {
    auto argListObj = GET_OBJ(args);
    assert(argListObj != nullptr);
    assert(argListObj->Is(ArgList::t));
    auto argList = (ArgList*)argListObj;

    std::cout << '\n';
    uint numOfArguments = argList->NumOfArguments();
    for (uint i = 0; i < numOfArguments; i++)
        std::cout << GET_OBJ(argList->Get(i))->ToStr();

    return Ref::none;
}

///////////////////////////////////////////////////////////////////////////////

Near::Near() : NFun{"near"} {}

v_real Near::Get_p_flt(Ref num) {
    auto * numObj = GET_OBJ(num);
    if (numObj->Is(Real::t))
        return ((Real*)numObj)->val;
    else
        return ((Int*)numObj)->val;
}

bool nearly_equal(v_real a, v_real b, v_real delta) {
    if (a == b) return true;

    v_real abs_a {std::abs(a)};
    v_real abs_b {std::abs(b)};

    if ((a > 0 && b > 0) || (a < 0 && b < 0)) {
        v_real min {fmin(abs_a, abs_b)};
        v_real max {fmax(abs_a, abs_b)};
        if ((max - min) / min < delta)
            return true;
    }
    return (abs_a < delta / 2 && abs_b < delta / 2);
}

Ref Near::Execute(Ref args) {
    auto argsObj = GET_OBJ(args);
    assert(argsObj != nullptr);
    assert(argsObj->Is(ArgList::t));
    auto argList = (ArgList*)argsObj;

    if (argList->NumOfArguments() < 3)
        return NEW_REF(new Err("Not enough arguments for 'near' function."));

    Ref a = argList->Get(0);
    auto * a_obj = GET_OBJ(a);
    if (!a_obj->Is(Int::t) && !a_obj->Is(Real::t))
        return NEW_REF(new Err("First argument for 'near' function is not numerical."));
    v_real a_ {Get_p_flt(a)};

    Ref b = argList->Get(1);
    auto * b_obj = GET_OBJ(b);
    if (!b_obj->Is(Int::t) && !b_obj->Is(Real::t))
        return NEW_REF(new Err("Second argument for 'near' function is not numerical."));
    v_real b_ {Get_p_flt(b)};

    Ref delta = argList->Get(2);
    auto * deltaObj = GET_OBJ(delta);
    if (!deltaObj->Is(Int::t) && !deltaObj->Is(Real::t))
        return NEW_REF(new Err("delta argument for 'near' function is not numerical."));
    v_real delta_ {Get_p_flt(delta)};

    bool result = nearly_equal(a_, b_, delta_);
    return BOOL_REF(result);
}

///////////////////////////////////////////////////////////////////////////////

Random::Random() : NFun{"random"} {}

Ref Random::Execute(Ref args) {
    auto argsObj = GET_OBJ(args);
    assert(argsObj->Is(ArgList::t));
    auto argList = (ArgList*)argsObj;

    if (argList->NumOfArguments() == 0)
        return NEW_REF(new Int(std::rand()));
    return NEW_REF(new Err("Wrong call for 'random' function."));
}

///////////////////////////////////////////////////////////////////////////////

Len::Len() : NFun{"len"} {}

Ref Len::Execute(Ref args) {
    auto argsObj = GET_OBJ(args);
    assert(argsObj->Is(ArgList::t));
    auto argList = (ArgList*)argsObj;

    if (argList->NumOfArguments() != 1)
        return NEW_REF(new Err("Wrong call for 'len' function."));

    auto * listObj = GET_OBJ(argList->Get(0));
    assert(listObj->Is(List::t));
    return NEW_REF(new Int(((List*)listObj)->elements.size()));
}

///////////////////////////////////////////////////////////////////////////////

CSeg::CSeg() : NFun{"seg"} {}

Ref CSeg::Execute(Ref args) {
    auto argsObj = GET_OBJ(args);
    assert(argsObj->Is(ArgList::t));
    auto argList = (ArgList*)argsObj;

    if (argList->NumOfArguments() < 2)
        return NEW_REF(new Err("'seg' function requires at least 2 parameters."));

    Ref a = argList->Get(0);
    Ref b = argList->Get(1);
    Ref inc;
    if (argList->NumOfArguments() == 2)
        inc = NEW_REF(new Int(1));
    else
        inc = argList->Get(2);
    return NEW_REF(new Seg(a, b, inc));
}

///////////////////////////////////////////////////////////////////////////////

VmStat::VmStat() : NFun{"VmStat"} {}

Ref VmStat::Execute(Ref args) {
    VM::PrintStatus();
    Mem::PrintMem();
    return Ref::none;
}

///////////////////////////////////////////////////////////////////////////////

CtxStat::CtxStat() : NFun{"CtxStat"} {}

Ref CtxStat::Execute(Ref args) {
    VM::contextStack.Last()->Print();
    return Ref::none;
}

///////////////////////////////////////////////////////////////////////////////

std::map<Ref, Ref> ZeroNamespace::builtins {};

void ZeroNamespace::CreateBuiltinObj(const std::string & name, Obj * obj) {
    Ref key = VM::GetConstant_Str(name);
    Ref val = NEW_PRESERVED_REF(obj);
    builtins[key] = val;
}

void ZeroNamespace::Init() {
    CreateBuiltinObj("assert",      new Assert());
    CreateBuiltinObj("print",       new Print());
    CreateBuiltinObj("pi",          new Real(3.14159265'35897932'38462643'38327950'2884L));
    CreateBuiltinObj("eu",          new Real(2.71828182'84590452'35360287'47135266'2497L));
    CreateBuiltinObj("near",        new Near());
    CreateBuiltinObj("random",      new Random());
    CreateBuiltinObj("len",         new Len());
    CreateBuiltinObj("seg",         new CSeg());
    CreateBuiltinObj("VmStat",      new VmStat());
    CreateBuiltinObj("CtxStat",     new CtxStat());
}

Ref ZeroNamespace::Get(Ref name) {
    auto * nameObj = GET_OBJ(name);
    assert(nameObj->Is(Str::t));
    if (builtins.count(name) > 0)
        return builtins[name];

    std::stringstream s;
    std::string nameStr = ((Str*)nameObj)->val;
    s << "No such identifier: \"" << nameStr << "\".";
    return NEW_REF(new Err(s.str()));
}

}
