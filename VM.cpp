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

const uint ExecStack::OBJ_STACK_MAX_SIZE = 8 * 1024 * 1024; // 8 Mb

const uint ExecStack::STACK_UNIT = 8;

ExecStack::ExecStack() {
    objStack = (std::byte*)calloc(OBJ_STACK_MAX_SIZE, 1);
}

ExecStack::~ExecStack() {
    free(objStack);
}

void ExecStack::CheckStackOverflow() {
    if (objStackTop > OBJ_STACK_MAX_SIZE) {
        std::cerr << '\n' << "Stack overflow.";
        abort();
    }
}

void ExecStack::NewContext() {
    CheckStackOverflow();
    auto * context = new Context();
    *((Context**)(objStack + objStackTop)) = context;
    frameStack.push(objStackTop);
    objStackTop += STACK_UNIT;
    lastContext = context;
}

void ExecStack::CloseContext() {
    uint lastFramePos = frameStack.top();
    frameStack.pop();
    auto * context = *((Context**)(objStack + lastFramePos));
    delete context;
    objStackTop = lastFramePos;

    if (frameStack.size() > 0) {
        lastFramePos = frameStack.top();
        lastContext = *((Context**)(objStack + lastFramePos));
    }
}

void ExecStack::PushObj(Obj * obj) {
    CheckStackOverflow();
    *((Obj**)(objStack + objStackTop)) = obj;
    objStackTop += STACK_UNIT;
}

Obj * ExecStack::PopObj() {
    assert(objStackTop > 0);
    objStackTop -= STACK_UNIT;
    return *((Obj**)(objStack + objStackTop));
}

Context * GetLastContext() {
    return lastContext;
}

void ExecStack::PrintFrames() {
    if (frameStack.empty())
        return;

    uint lastFramePos = frameStack.top();
    auto * context = *((Context**)(objStack + lastFramePos));
    context->Print();
}

///////////////////////////////////////////////////////////////////////////////

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
    auto * method = obj->type->methodTable->DebugStr;
    if (method == nullptr) {
        objStr = obj->type->name;
    } else {
        objStr = method(obj);
    }
    return objStr;
}

void VM::Execute(const ByteCode & byteCode) {
    ByteCodeReader bcr(byteCode);
    ExecStack execStack;
    for (;;)
    {
        OpCode opCode = bcr.Read_OpCode();
        switch (opCode)
        {
            case OpCode::NoOperation:
                break;

            case OpCode::NewContext:
            {
                execStack.NewContext();
                break;
            }

            case OpCode::CloseContext:
            {
                execStack.CloseContext();
                break;
            }

            case OpCode::PushConstant:
            {
                OpArg id = bcr.Read_OpArg();
                auto * constantObj = GetConstantById(id);
                execStack.PushObj(constantObj);
                break;
            }

            case OpCode::GetLocalVariable:
            {
                OpArg  id      = bcr.Read_OpArg();
                Obj  * name    = GetConstantById(id);
                auto * context = execStack.GetLastContext();
                Obj  * result  = context->GetVariable(name);
                HandlePossibleError(result);
                execStack.PushObj(result);
                break;
            }

            case OpCode::SetLocalVariable:
            {
                OpArg  id      = bcr.Read_OpArg();
                Obj  * name    = GetConstantById(id);
                auto * obj     = (Obj*)objStack[objStackTop];
                auto * context = (Context*)objStack[frameStack.top()];
                auto * result  = context->SetVariable(name, obj);
                HandlePossibleError(result);
                objStackTop--;
                break;
            }

            case OpCode::Equal:
            {
                auto * obj_2  = (Obj*)objStack[objStackTop];
                auto * obj_1  = (Obj*)objStack[objStackTop - 1];
                auto * method = obj_1->type->methodTable->Equal;
                if (method == nullptr) {
                    ThrowError_NoSuchOperation(obj_1->type, "'='");
                }
                auto * result = method(obj_1, obj_2);
                HandlePossibleError(result);
                objStackTop--;
                objStack[objStackTop] = result;
                break;
            }

            case OpCode::NotEqual:
            {
                auto * obj_2  = (Obj*)objStack[objStackTop];
                auto * obj_1  = (Obj*)objStack[objStackTop - 1];
                auto * method = obj_1->type->methodTable->Equal;
                if (method == nullptr) {
                    ThrowError_NoSuchOperation(obj_1->type, "'!='");
                }
                auto * result = method(obj_1, obj_2);
                HandlePossibleError(result);
                objStackTop--;
                objStack[objStackTop] = ((Bool*)result)->Invert();
                break;
            }

            case OpCode::Negate:
            {
                auto * obj    = (Obj*)objStack[objStackTop];
                auto * method = obj->type->methodTable->Negate;
                if (method == nullptr) {
                    ThrowError_NoSuchOperation(obj->type, "'-' (negation)");
                }
                auto * result = method(obj);
                HandlePossibleError(result);
                objStack[objStackTop] = result;
                break;
            }

            case OpCode::Add:
            {
                auto * obj_2  = (Obj*)objStack[objStackTop];
                auto * obj_1  = (Obj*)objStack[objStackTop - 1];
                auto * method = obj_1->type->methodTable->Add;
                if (method == nullptr) {
                    ThrowError_NoSuchOperation(obj_1->type, "'+'");
                }
                auto * result = method(obj_1, obj_2);
                HandlePossibleError(result);
                objStackTop--;
                objStack[objStackTop] = result;
                break;
            }

            case OpCode::Subtract:
            {
                auto * obj_2  = (Obj*)objStack[objStackTop];
                auto * obj_1  = (Obj*)objStack[objStackTop - 1];
                auto * method = obj_1->type->methodTable->Subtract;
                if (method == nullptr) {
                    ThrowError_NoSuchOperation(obj_1->type, "'-'");
                }
                auto * result = method(obj_1, obj_2);
                HandlePossibleError(result);
                objStackTop--;
                objStack[objStackTop] = result;
                break;
            }

            case OpCode::Multiply:
            {
                auto * obj_2  = (Obj*)objStack[objStackTop];
                auto * obj_1  = (Obj*)objStack[objStackTop - 1];
                auto * method = obj_1->type->methodTable->Multiply;
                if (method == nullptr) {
                    ThrowError_NoSuchOperation(obj_1->type, "'*'");
                }
                auto * result = method(obj_1, obj_2);
                HandlePossibleError(result);
                objStackTop--;
                objStack[objStackTop] = result;
                break;
            }

            case OpCode::Divide:
            {
                auto * obj_2  = (Obj*)objStack[objStackTop];
                auto * obj_1  = (Obj*)objStack[objStackTop - 1];
                auto * method = obj_1->type->methodTable->Divide;
                if (method == nullptr) {
                    ThrowError_NoSuchOperation(obj_1->type, "'/'");
                }
                auto * result = method(obj_1, obj_2);
                HandlePossibleError(result);
                objStackTop--;
                objStack[objStackTop] = result;
                break;
            }

            case OpCode::Power:
            {
                auto * obj_2  = (Obj*)objStack[objStackTop];
                auto * obj_1  = (Obj*)objStack[objStackTop - 1];
                auto * method = obj_1->type->methodTable->Power;
                if (method == nullptr) {
                    ThrowError_NoSuchOperation(obj_1->type, "'^'");
                }
                auto * result = method(obj_1, obj_2);
                HandlePossibleError(result);
                objStackTop--;
                objStack[objStackTop] = result;
                break;
            }

            case OpCode::Greater:
            {
                auto * obj_2  = (Obj*)objStack[objStackTop];
                auto * obj_1  = (Obj*)objStack[objStackTop - 1];
                auto * method = obj_1->type->methodTable->Greater;
                if (method == nullptr) {
                    ThrowError_NoSuchOperation(obj_1->type, "'>'");
                }
                auto * result = method(obj_1, obj_2);
                HandlePossibleError(result);
                objStackTop--;
                objStack[objStackTop] = result;
                break;
            }

            case OpCode::GreaterOrEqual:
            {
                auto * obj_2  = (Obj*)objStack[objStackTop];
                auto * obj_1  = (Obj*)objStack[objStackTop - 1];
                auto * method = obj_1->type->methodTable->GreaterOrEqual;
                if (method == nullptr) {
                    ThrowError_NoSuchOperation(obj_1->type, "'>='");
                }
                auto * result = method(obj_1, obj_2);
                HandlePossibleError(result);
                objStackTop--;
                objStack[objStackTop] = result;
                break;
            }

            case OpCode::Less:
            {
                auto * obj_2  = (Obj*)objStack[objStackTop];
                auto * obj_1  = (Obj*)objStack[objStackTop - 1];
                auto * method = obj_1->type->methodTable->Less;
                if (method == nullptr) {
                    ThrowError_NoSuchOperation(obj_1->type, "'<'");
                }
                auto * result = method(obj_1, obj_2);
                HandlePossibleError(result);
                objStackTop--;
                objStack[objStackTop] = result;
                break;
            }

            case OpCode::LessOrEqual:
            {
                auto * obj_2  = (Obj*)objStack[objStackTop];
                auto * obj_1  = (Obj*)objStack[objStackTop - 1];
                auto * method = obj_1->type->methodTable->LessOrEqual;
                if (method == nullptr) {
                    ThrowError_NoSuchOperation(obj_1->type, "'<='");
                }
                auto * result = method(obj_1, obj_2);
                HandlePossibleError(result);
                objStackTop--;
                objStack[objStackTop] = result;
                break;
            }

            case OpCode::Not:
            {
                auto * obj    = (Obj*)objStack[objStackTop];
                auto * result = Bool::Not(obj);
                HandlePossibleError(result);
                objStack[objStackTop] = result;
                break;
            }

            case OpCode::And:
            {
                auto * obj_2  = (Obj*)objStack[objStackTop];
                auto * obj_1  = (Obj*)objStack[objStackTop - 1];
                auto * result = Bool::And(obj_1, obj_2);
                HandlePossibleError(result);
                objStackTop--;
                objStack[objStackTop] = result;
                break;
            }

            case OpCode::Or:
            {
                auto * obj_2  = (Obj*)objStack[objStackTop];
                auto * obj_1  = (Obj*)objStack[objStackTop - 1];
                auto * result = Bool::Or(obj_1, obj_2);
                HandlePossibleError(result);
                objStackTop--;
                objStack[objStackTop] = result;
                break;
            }

            case OpCode::Jump:
            {
                bcr.Read_OpArg_SetAsPos();
                break;
            }

            case OpCode::JumpIfFalse:
            {
                auto * obj = (Obj*)objStack[objStackTop];
                if (obj->type != Bool::t) {
                    ThrowError("Condition result must be of a boolean type.");
                }
                objStackTop--;
                if ((Bool*)obj == Bool::True) {
                    bcr.Skip_OpArg();
                    break;
                }
                bcr.Read_OpArg_SetAsPos();
                break;
            }

            case OpCode::Assert:
            {
                auto * obj_3 = (Obj*)objStack[objStackTop];     // message
                auto * obj_2 = (Obj*)objStack[objStackTop - 1]; // line
                auto * obj_1 = (Obj*)objStack[objStackTop - 2]; // bool

                if (obj_1->type != Bool::t)
                    ThrowError("Asserting expression must be of a boolean type.");

                if ((Bool*)obj_1 == Bool::False) {
                    assert(obj_2->Is(Int::t));
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

            case OpCode::PushInt32:
            {
                int32_t i = bcr.Read_int32();
                break;
            }

            case OpCode::SaveByteCodePosition:
            {
                /*
                OpArg  id      = bcr.Read_OpArg();
                Obj  * name    = GetConstantById(id);
                auto * context = (Context*)objStack[frameStack.top()];
                Obj  * result  = context->GetVariable(name);
                HandlePossibleError(result);
                objStackTop++;
                objStack[objStackTop] = result;
                break;
                */
                break;
            }

            default :
                std::cerr << "\nFatal error: Unknown OpCode.";
                abort();
        }
        if (bcr.IsAtEnd())
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
      << "' does not provide operation "
      << opSymbol
      << ".";
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
        auto * method = val->type->methodTable->DebugStr;
        if (method == nullptr) {
            valStr = val->type->name;
        } else {
            valStr = method(val);
        }
        std::cout << '\n' << i << " : " << valStr;
    }
    std::cout << '\n';
}
