#ifndef PROTON_VM_H
#define PROTON_VM_H

#include <map>
#include <cstdlib>
#include <iostream>
#include "Mem.h"
#include "Obj.h"
#include "ByteCode.h"

struct VM {
    static uint                        nextId;
    static std::vector<Obj*>           constants;
    static std::map<v_int, uint>       constantsId_Int;
    static std::map<v_real, uint>      constantsId_Real;
    static std::map<std::string, uint> constantsId_Str;

    static std::array<void*, 1024> ptrStack;
    static int ptrStackTop;
    static std::stack<uint> frameStack;

    static void Init();

    static uint  GetConstantId_Int(v_int val);
    static uint  GetConstantId_Real(v_real val);
    static uint  GetConstantId_Str(const std::string & val);
    static Obj * GetConstant(uint id);

    static void Execute(const ByteCode & bc);
};

#endif //PROTON_VM_H
