#ifndef PROTON_VM_H
#define PROTON_VM_H

#include "Mem.h"
//#include "ContextStack.h"

struct VM {
    /*
    static ContextStack contextStack;
     */
    static void Init();
    /*
    static void Mark();
    static void PrintStatus();

    static std::map<v_int, Ref>       constants_Int;
    static std::map<v_real, Ref>      constants_Real;
    static std::map<std::string, Ref> constants_Str;

    [[nodiscard]]
    static Ref GetConstant_Int(v_int val);

    [[nodiscard]]
    static Ref GetConstant_Real(v_real val);

    [[nodiscard]]
    static Ref GetConstant_Str(const std::string & val);
     */
};

#endif //PROTON_VM_H
