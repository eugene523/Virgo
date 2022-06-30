#include <sstream>
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
    VM::NoneId = GetConstantId_Obj((Obj*)None::none);

    Error::InitType();

    Bool::InitType();
    VM::TrueId  = GetConstantId_Obj((Obj*)Bool::True);
    VM::FalseId = GetConstantId_Obj((Obj*)Bool::False);

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
uint                        VM::NoneId;
uint                        VM::TrueId;
uint                        VM::FalseId;
std::array<void*, 1024>     VM::objStack;
int                         VM::objStackTop = -1;
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

uint VM::GetConstantId_Obj(Obj * obj) {
    constants.push_back(obj);
    uint id = nextId;
    nextId++;
    return id;
}

Obj * VM::GetConstantById(uint id) {
    return constants.at(id);
}

std::string VM::ConstantToStr(uint id) {
    Obj * obj = GetConstantById(id);
    std::string objStr;
    auto * method = obj->type->methodTable->Dstr;
    if (method == nullptr) {
        objStr = obj->type->name;
    } else {
        objStr = method(obj);
    }
    return objStr;
}

void VM::Execute(const ByteCode & byteCode) {
    std::byte * bc = byteCode.bc;
    uint pos = 0;
    for (;;)
    {
        OpCode opCode = *((OpCode*)(bc + pos));
        pos += sizeof(OpCode);
        switch (opCode)
        {
            case OpCode::Noop :
                break;

            case OpCode::NewFrame :
            {
                objStackTop++;
                objStack[objStackTop] = new Context();
                frameStack.push(objStackTop);
                break;
            }

            case OpCode::CloseFrame :
            {
                int lastFramePos = frameStack.top();
                frameStack.pop();
                delete ((Context*)objStack[lastFramePos]);
                objStackTop = lastFramePos - 1;
                break;
            }

            case OpCode::LoadConstant :
            {
                uint64_t id = *((uint64_t*)(bc + pos));
                pos += sizeof(uint64_t);
                objStackTop++;
                objStack[objStackTop] = GetConstantById(id);
                break;
            }

            case OpCode::GetLocalVariable :
            {
                uint64_t id = *((uint64_t*)(bc + pos));
                pos += sizeof(uint64_t);
                Obj  * name    = GetConstantById(id);
                auto * context = (Context*)objStack[frameStack.top()];
                Obj  * result  = context->GetVariable(name);
                HandlePossibleError(result);
                objStackTop++;
                objStack[objStackTop] = result;
                break;
            }

            case OpCode::SetLocalVariable :
            {
                uint64_t id = *((uint64_t*)(bc + pos));
                pos += sizeof(uint64_t);
                Obj  * name    = GetConstantById(id);
                auto * obj     = (Obj*)objStack[objStackTop];
                auto * context = (Context*)objStack[frameStack.top()];
                auto * result  = context->SetVariable(name, obj);
                HandlePossibleError(result);
                objStackTop--;
                break;
            }

            case OpCode::Eq :
            {
                auto * obj_2  = (Obj*)objStack[objStackTop];
                auto * obj_1  = (Obj*)objStack[objStackTop - 1];
                auto * method = obj_1->type->methodTable->OpEq;
                if (method == nullptr) {
                    ThrowError_NoSuchOperation(obj_1->type, "=");
                }
                auto * result = method(obj_1, obj_2);
                HandlePossibleError(result);
                objStackTop--;
                objStack[objStackTop] = result;
                break;
            }

            case OpCode::NotEq :
            {
                auto * obj_2  = (Obj*)objStack[objStackTop];
                auto * obj_1  = (Obj*)objStack[objStackTop - 1];
                auto * method = obj_1->type->methodTable->OpNotEq;
                if (method == nullptr) {
                    ThrowError_NoSuchOperation(obj_1->type, "!=");
                }
                auto * result = method(obj_1, obj_2);
                HandlePossibleError(result);
                objStackTop--;
                objStack[objStackTop] = result;
                break;
            }

            case OpCode::Neg :
            {
                auto * obj    = (Obj*)objStack[objStackTop];
                auto * method = obj->type->methodTable->OpNeg;
                if (method == nullptr) {
                    ThrowError_NoSuchOperation(obj->type, "- (negation)");
                }
                auto * result = method(obj);
                HandlePossibleError(result);
                objStack[objStackTop] = result;
                break;
            }

            case OpCode::Add :
            {
                auto * obj_2  = (Obj*)objStack[objStackTop];
                auto * obj_1  = (Obj*)objStack[objStackTop - 1];
                auto * method = obj_1->type->methodTable->OpAdd;
                if (method == nullptr) {
                    ThrowError_NoSuchOperation(obj_1->type, "+");
                }
                auto * result = method(obj_1, obj_2);
                HandlePossibleError(result);
                objStackTop--;
                objStack[objStackTop] = result;
                break;
            }

            case OpCode::Sub :
            {
                auto * obj_2  = (Obj*)objStack[objStackTop];
                auto * obj_1  = (Obj*)objStack[objStackTop - 1];
                auto * method = obj_1->type->methodTable->OpSub;
                if (method == nullptr) {
                    ThrowError_NoSuchOperation(obj_1->type, "-");
                }
                auto * result = method(obj_1, obj_2);
                HandlePossibleError(result);
                objStackTop--;
                objStack[objStackTop] = result;
                break;
            }

            case OpCode::Mul :
            {
                auto * obj_2  = (Obj*)objStack[objStackTop];
                auto * obj_1  = (Obj*)objStack[objStackTop - 1];
                auto * method = obj_1->type->methodTable->OpMul;
                if (method == nullptr) {
                    ThrowError_NoSuchOperation(obj_1->type, "*");
                }
                auto * result = method(obj_1, obj_2);
                HandlePossibleError(result);
                objStackTop--;
                objStack[objStackTop] = result;
                break;
            }

            case OpCode::Div :
            {
                auto * obj_2  = (Obj*)objStack[objStackTop];
                auto * obj_1  = (Obj*)objStack[objStackTop - 1];
                auto * method = obj_1->type->methodTable->OpDiv;
                if (method == nullptr) {
                    ThrowError_NoSuchOperation(obj_1->type, "/");
                }
                auto * result = method(obj_1, obj_2);
                HandlePossibleError(result);
                objStackTop--;
                objStack[objStackTop] = result;
                break;
            }

            case OpCode::Pow :
            {
                auto * obj_2  = (Obj*)objStack[objStackTop];
                auto * obj_1  = (Obj*)objStack[objStackTop - 1];
                auto * method = obj_1->type->methodTable->OpPow;
                if (method == nullptr) {
                    ThrowError_NoSuchOperation(obj_1->type, "^");
                }
                auto * result = method(obj_1, obj_2);
                HandlePossibleError(result);
                objStackTop--;
                objStack[objStackTop] = result;
                break;
            }

            case OpCode::Gr :
            {
                auto * obj_2  = (Obj*)objStack[objStackTop];
                auto * obj_1  = (Obj*)objStack[objStackTop - 1];
                auto * method = obj_1->type->methodTable->OpGr;
                if (method == nullptr) {
                    ThrowError_NoSuchOperation(obj_1->type, ">");
                }
                auto * result = method(obj_1, obj_2);
                HandlePossibleError(result);
                objStackTop--;
                objStack[objStackTop] = result;
                break;
            }

            case OpCode::GrEq :
            {
                auto * obj_2  = (Obj*)objStack[objStackTop];
                auto * obj_1  = (Obj*)objStack[objStackTop - 1];
                auto * method = obj_1->type->methodTable->OpGrEq;
                if (method == nullptr) {
                    ThrowError_NoSuchOperation(obj_1->type, ">=");
                }
                auto * result = method(obj_1, obj_2);
                HandlePossibleError(result);
                objStackTop--;
                objStack[objStackTop] = result;
                break;
            }

            case OpCode::Ls :
            {
                auto * obj_2  = (Obj*)objStack[objStackTop];
                auto * obj_1  = (Obj*)objStack[objStackTop - 1];
                auto * method = obj_1->type->methodTable->OpLs;
                if (method == nullptr) {
                    ThrowError_NoSuchOperation(obj_1->type, "<");
                }
                auto * result = method(obj_1, obj_2);
                HandlePossibleError(result);
                objStackTop--;
                objStack[objStackTop] = result;
                break;
            }

            case OpCode::LsEq :
            {
                auto * obj_2  = (Obj*)objStack[objStackTop];
                auto * obj_1  = (Obj*)objStack[objStackTop - 1];
                auto * method = obj_1->type->methodTable->OpLsEq;
                if (method == nullptr) {
                    ThrowError_NoSuchOperation(obj_1->type, "<=");
                }
                auto * result = method(obj_1, obj_2);
                HandlePossibleError(result);
                objStackTop--;
                objStack[objStackTop] = result;
                break;
            }

            case OpCode::Not :
            {
                auto * obj    = (Obj*)objStack[objStackTop];
                auto * method = obj->type->methodTable->OpNot;
                if (method == nullptr) {
                    ThrowError_NoSuchOperation(obj->type, "not");
                }
                auto * result = method(obj);
                HandlePossibleError(result);
                objStack[objStackTop] = result;
                break;
            }

            case OpCode::And :
            {
                auto * obj_2  = (Obj*)objStack[objStackTop];
                auto * obj_1  = (Obj*)objStack[objStackTop - 1];
                auto * method = obj_1->type->methodTable->OpAnd;
                if (method == nullptr) {
                    ThrowError_NoSuchOperation(obj_1->type, "and");
                }
                auto * result = method(obj_1, obj_2);
                HandlePossibleError(result);
                objStackTop--;
                objStack[objStackTop] = result;
                break;
            }

            case OpCode::Or :
            {
                auto * obj_2  = (Obj*)objStack[objStackTop];
                auto * obj_1  = (Obj*)objStack[objStackTop - 1];
                auto * method = obj_1->type->methodTable->OpOr;
                if (method == nullptr) {
                    ThrowError_NoSuchOperation(obj_1->type, "or");
                }
                auto * result = method(obj_1, obj_2);
                HandlePossibleError(result);
                objStackTop--;
                objStack[objStackTop] = result;
                break;
            }

            case OpCode::Jump :
            {
                uint64_t bcPos = *((uint64_t*)(bc + pos));
                pos = bcPos;
                break;
            }

            case OpCode::JumpFalse :
            {
                auto * obj = (Obj*)objStack[objStackTop];
                if (obj->type != Bool::t) {
                    ThrowError("Condition result must be of a boolean type.");
                }
                objStackTop--;
                if ((Bool*)obj == Bool::True) {
                    pos += sizeof(uint64_t);
                    break;
                }
                uint64_t posFalse = *((uint64_t*)(bc + pos));
                pos = posFalse;
                break;
            }

            case OpCode::Assert :
            {
                auto * obj_3 = (Obj*)objStack[objStackTop];     // message
                auto * obj_2 = (Obj*)objStack[objStackTop - 1]; // line
                auto * obj_1 = (Obj*)objStack[objStackTop - 2]; // bool

                if (obj_1->type != Bool::t)
                    ThrowError("Asserting expression must be of a boolean type.");

                if ((Bool*)obj_1 == Bool::False) {
                    std::stringstream s;
                    v_int line = ((Int*)obj_2)->val;
                    s << "\nLine " << line << ". Assertion failed. ";
                    if (obj_3 != (Obj*)None::none) {
                        const char * message = ((Str*)obj_3)->val;
                        s << message;
                    }
                    ThrowError(s.str());
                }
                objStackTop -= 3;
                break;
            }

            default :
                std::cerr << "\nFatal error: Unknown OpCode.";
                abort();
        }
        if (pos >= byteCode.bcPos)
            break;
    }
}

void VM::HandlePossibleError(Obj * obj) {
    if (obj == nullptr)
        return;

    if (!obj->Is(Error::t))
        return;

    std::cerr << '\n' << ((Error*)obj)->message;
    abort();
}

void VM::ThrowError(const std::string & message) {
    std::cerr << '\n' << message;
    abort();
}

void VM::ThrowError_NoSuchOperation(const Type * t, const std::string & opSymbol) {
    std::stringstream s;
    s << "\nObject of type '"
      << t->name
      << "' does not provide operation '"
      << opSymbol
      << "'.";
    ThrowError(s.str());
}

void VM::PrintFrames() {
    if (objStackTop == -1)
        return;
    auto * context = (Context*)objStack[frameStack.top()];
    context->Print();
}

void VM::PrintConstants() {
    std::cout << "\n\nConstants (" << nextId << ")";
    for (uint i = 0; i < nextId; i++) {
        std::string valStr;
        auto * val = constants[i];
        auto * method = val->type->methodTable->Dstr;
        if (method == nullptr) {
            valStr = val->type->name;
        } else {
            valStr = method(val);
        }
        std::cout << '\n' << i << " : " << valStr;
    }
    std::cout << '\n';
}
