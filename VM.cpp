#include "VM.h"
#include "Type.h"
#include "None.h"
#include "Error.h"
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

/*
ContextStack VM::contextStack;
*/

void VM::Init() {
    /*
    Mem::Init();
    Mem::MarkCallback = &Mark;

    None::InitType();
    Error::InitType();
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
     */
}

uint                        VM::nextId;
std::map<uint, Obj*>        VM::constants;
std::map<v_int, uint>       VM::constantsId_Int{};
std::map<v_real, uint>      VM::constantsId_Real{};
std::map<std::string, uint> VM::constantsId_Str{};

uint VM::GetConstantId_Int(v_int val) {
    if (constantsId_Int.count(val) > 0)
        return constantsId_Int[val];

    void * inPlace = Heap::GetChunk_Constant(sizeof(Int));
    Int::New(inPlace, val);
    uint id = nextId;
    nextId++;
    constants[id] = (Obj*)inPlace;
    constantsId_Int[val] = id;
    return id;
}

uint VM::GetConstantId_Real(v_real val) {
    if (constantsId_Real.count(val) > 0)
        return constantsId_Real[val];

    void * inPlace = Heap::GetChunk_Constant(sizeof(Real));
    Real::New(inPlace, val);
    uint id = nextId;
    nextId++;
    constants[id] = (Obj*)inPlace;
    constantsId_Real[val] = id;
    return id;
}

uint VM::GetConstantId_Str(const std::string & val) {
    if (constantsId_Str.count(val) > 0)
        return constantsId_Str[val];

    void * inPlace = Heap::GetChunk_Constant(sizeof(Real));
    Str::New(inPlace, val.c_str());
    uint id = nextId;
    nextId++;
    constants[id] = (Obj*)inPlace;
    constantsId_Str[val] = id;
    return id;
}
