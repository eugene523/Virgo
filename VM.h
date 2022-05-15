#ifndef PROTON_VM_H
#define PROTON_VM_H

#include <map>
#include "Mem.h"
#include "Obj.h"

struct VM {
    static uint                        nextId;
    static std::map<uint, Obj*>        constants;
    static std::map<v_int, uint>       constantsId_Int;
    static std::map<v_real, uint>      constantsId_Real;
    static std::map<std::string, uint> constantsId_Str;

    static std::array<Obj*, 1024> stack;
    static int stackTop;

    static void Init();

    static uint GetConstantId_Int(v_int val);
    static uint GetConstantId_Real(v_real val);
    static uint GetConstantId_Str(const std::string & val);
    static Obj* GetConstant(uint id);

};

#endif //PROTON_VM_H
