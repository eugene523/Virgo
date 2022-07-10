#ifndef PROTON_VM_H
#define PROTON_VM_H

#include <map>
#include <cstdlib>
#include <iostream>
#include <cassert>
#include "Mem.h"
#include "Obj.h"
#include "Error.h"
#include "ByteCode.h"

struct ExecStack
{
    std::byte *       objStack{};
    uint              objStackTop{};
    static const uint OBJ_STACK_MAX_SIZE;
    static const uint CELL_SIZE;
    std::stack<uint>  frameStack;
    Context *         lastContext{};

    ExecStack();
    ~ExecStack();

    void CheckStackOverflow();
    void NewContext();
    void CloseContext();
    void PushObj(Obj * obj);
    Obj * PopObj();
    Context * GetLastContext();
    void PrintFrames();
};

///////////////////////////////////////////////////////////////////////////////

struct VM {
    static uint                        nextId;
    static std::vector<Obj*>           constants;
    static std::map<v_int, uint>       constantsId_Int;
    static std::map<v_real, uint>      constantsId_Real;
    static std::map<std::string, uint> constantsId_Str;
    static uint                        NoneId;
    static uint                        TrueId;
    static uint                        FalseId;

    static std::array<void*, 1024> objStack;
    static int                     objStackTop;
    static std::stack<uint>        frameStack;

    static void Init();

    static uint  GetConstantId_Int(v_int val);
    static uint  GetConstantId_Real(v_real val);
    static uint  GetConstantId_Str(const std::string & val);
    static uint  GetConstantId_Obj(Obj * obj);
    static Obj * GetConstantById(uint id);
    static std::string ConstantToStr(uint id);

    static void Execute(const ByteCode & bc);
    static void HandlePossibleError(Obj * obj);
    static void ThrowError(const std::string & message);
    static void ThrowError_NoSuchOperation(const Type * t, const std::string & opSymbol);

    static void PrintConstants();
    static void PrintFrames();
};

#endif //PROTON_VM_H
