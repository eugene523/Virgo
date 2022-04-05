#include "VM.h"
#include "Type.h"
#include "None.h"
#include "Err.h"
#include "Bool.h"
#include "Int.h"
#include "Real.h"
#include "Str.h"
#include "List.h"
#include "Seg.h"
#include "Iter.h"
#include "Args.h"
#include "ArgPair.h"
#include "Skip.h"
#include "Break.h"
#include "Return.h"
#include "Fun.h"
#include "Object.h"
#include "Invoker.h"
#include "Script.h"
#include "Builtins.h"

ContextStack VM::contextStack;

void VM::Init() {
    Mem::Init();
    Mem::MarkCallback = &Mark;

    None::InitType();
    Err::InitType();
    Bool::InitType();
    Int::InitType();
    Real::InitType();
    Str::InitType();
    List::InitType();
    Seg::InitType();
    Iter::InitType();
    Args::InitType();
    ArgPair::InitType();
    Skip::InitType();
    Break::InitType();
    Return::InitType();
    Fun::InitType();
    NFun::InitType();
    Object::InitType();
    Invoker::InitType();
    Script::InitType();

    Ref none    = NEW_PRESERVED_REF(new None());
    Bool::True  = NEW_PRESERVED_REF(new Bool(true));
    Bool::False = NEW_PRESERVED_REF(new Bool(false));

    Builtins::ZeroNamespace::Init();
}

void VM::Mark() {
    contextStack.Mark();
}

void VM::PrintStatus() {
    Mem::PrintStatus();
}

std::map<v_int, Ref>       VM::constants_Int{};
std::map<v_real, Ref>      VM::constants_Real{};
std::map<std::string, Ref> VM::constants_Str{};

Ref VM::GetConstant_Int(v_int val) {
    if (constants_Int.count(val) > 0)
        return constants_Int[val];
    Ref ref = NEW_PRESERVED_REF(new Int(val));
    constants_Int[val] = ref;
    return ref;
}

Ref VM::GetConstant_Real(v_real val) {
    if (constants_Real.count(val) > 0)
        return constants_Real[val];
    Ref ref = NEW_PRESERVED_REF(new Real(val));
    constants_Real[val] = ref;
    return ref;
}

Ref VM::GetConstant_Str(const std::string & val) {
    if (constants_Str.count(val) > 0)
        return constants_Str[val];
    Ref ref = NEW_PRESERVED_REF(new Str(val));
    constants_Str[val] = ref;
    return ref;
}
