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
int                         VM::ptrStackTop = -1;
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
        OpCode opCode = *((OpCode*)(bc.stream + pos));
        pos += sizeof(OpCode);
        switch (opCode)
        {
            case OpCode::NewFrame :
            {
                ptrStackTop++;
                ptrStack[ptrStackTop] = new Context();
                frameStack.push(ptrStackTop);
                break;
            }
            case OpCode::CloseFrame :
            {
                int lastFramePos = frameStack.top();
                frameStack.pop();
                delete ((Context*)ptrStack[lastFramePos]);
                ptrStackTop = lastFramePos - 1;
                break;
            }
            case OpCode::PushConstant :
            {
                uint64_t id = *((uint64_t*)(bc.stream + pos));
                pos += sizeof(uint64_t);
                ptrStackTop++;
                ptrStack[ptrStackTop] = GetConstant(id);
                break;
            }
            case OpCode::GetValueByName :
            {
                auto * name = (Obj*)ptrStack[ptrStackTop];
                auto * context = (Context*)ptrStack[frameStack.top()];
                Obj  * value = context->GetVariable(name);
                if (value->Is(Error::t)) {
                    std::cerr << ((Error*)value)->message;
                    abort();
                }
                ptrStack[ptrStackTop] = value;
                break;
            }
            case OpCode::SetValueByName :
            {
                auto * obj     = (Obj*)ptrStack[ptrStackTop];
                auto * name    = (Obj*)ptrStack[ptrStackTop - 1];
                auto * context = (Context*)ptrStack[frameStack.top()];
                auto * result  = context->SetVariable(name, obj);
                if (result != nullptr) {
                    assert(result->Is(Error::t));
                    std::cerr << ((Error*)result)->message;
                    abort();
                }
                ptrStackTop -= 2;
                break;
            }
            case OpCode::Add :
            {
                auto * obj_2  = (Obj*)ptrStack[ptrStackTop];
                auto * obj_1  = (Obj*)ptrStack[ptrStackTop - 1];
                auto * method = obj_1->type->methodTable->OpAdd;
                if (method == nullptr) {
                    std::cerr << "Object does not provide operation +.";
                    abort();
                }
                auto * result = method(obj_1, obj_2);
                if (result->Is(Error::t)) {
                    std::cerr << ((Error*)result)->message;
                    abort();
                }
                ptrStackTop--;
                ptrStack[ptrStackTop] = result;
                break;
            }
        }
        if (pos >= bc.pos)
            break;
    }
}

void VM::PrintFrames() {
    auto * context = (Context*)ptrStack[frameStack.top()];
    context->Print();
}