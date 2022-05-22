#include "VM.h"
#include "Type.h"
#include "None.h"
#include "Error.h"
#include "Bool.h"
#include "Int.h"
#include "Real.h"
#include "Str.h"
#include "Context.h"
/*
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
*/

void VM::Init() {
    Heap::Init();
    //Heap::PreDomainGc = ?;
    //Heap::PreGlobalGc = ?;

    None::InitType();
    Error::InitType();
    Bool::InitType();
    Int::InitType();
    Real::InitType();
    Str::InitType();
    /*
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

    Builtins::ZeroNamespace::Init();
    */
}

uint                        VM::nextId;
std::vector<Obj*>           VM::constants;
std::map<v_int, uint>       VM::constantsId_Int{};
std::map<v_real, uint>      VM::constantsId_Real{};
std::map<std::string, uint> VM::constantsId_Str{};
std::array<void*, 1024>     VM::ptrStack;
int                         VM::ptrStackTop;
std::stack<uint>            VM::frameStack;

uint VM::GetConstantId_Int(v_int val) {
    if (constantsId_Int.count(val) > 0)
        return constantsId_Int[val];

    void * inPlace = Heap::GetChunk_Constant(sizeof(Int));
    Int::New(inPlace, val);
    constants.push_back((Obj*)inPlace);
    uint id = nextId;
    nextId++;
    constantsId_Int[val] = id;
    return id;
}

uint VM::GetConstantId_Real(v_real val) {
    if (constantsId_Real.count(val) > 0)
        return constantsId_Real[val];

    void * inPlace = Heap::GetChunk_Constant(sizeof(Real));
    Real::New(inPlace, val);
    constants.push_back((Obj*)inPlace);
    uint id = nextId;
    nextId++;
    constantsId_Real[val] = id;
    return id;
}

uint VM::GetConstantId_Str(const std::string & val) {
    if (constantsId_Str.count(val) > 0)
        return constantsId_Str[val];

    void * inPlace = Heap::GetChunk_Constant(sizeof(Real));
    Str::New(inPlace, val.c_str());
    constants.push_back((Obj*)inPlace);
    uint id = nextId;
    nextId++;
    constantsId_Str[val] = id;
    return id;
}

Obj * VM::GetConstant(uint id) {
    return constants.at(id);
}

void VM::Execute(const ByteCode & bc) {
    uint pos = 0;
    for (;;)
    {
        OpCode op = *((OpCode*)(bc.stream + pos));
        pos += sizeof(OpCode);
        switch (op)
        {
            case OpCode::NewFrame :
            {
                ptrStack[ptrStackTop] = new Context();
                frameStack.push(ptrStackTop);
                ptrStackTop++;
                break;
            }
            case OpCode::CloseFrame :
            {
                uint lastFramePos = frameStack.top();
                frameStack.pop();
                delete ((Context*)ptrStack[lastFramePos]);
                ptrStackTop = lastFramePos;
            }
            case OpCode::PushConstant :
            {
                uint64_t id = *((uint64_t*)(bc.stream + pos));
                pos += sizeof(uint64_t);
                ptrStack[ptrStackTop] = GetConstant(id);
                ptrStackTop++;
            }
            case OpCode::GetValueByName :
            {
                auto * name = (Obj*)ptrStack[ptrStackTop - 1];
                auto * context = (Context*)ptrStack[frameStack.top()];
                Obj  * value = context->GetVariable(name);
                if (value->Is(Error::t)) {
                    std::cerr << ((Error*)value)->message;
                    abort();
                }
                ptrStack[ptrStackTop - 1] = value;
            }
        }
        if (pos >= bc.pos)
            break;
    }
}