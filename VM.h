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

    // Temporary references stack is used during evaluating expressions.
    // We push to this stack intermediate results of a currently evaluating expression,
    // so we can be sure that intermediate values will not be deleted if GC will start
    // in the middle of expression.
    /*
    static const uint TEMP_STACK_CAPACITY;
    static std::list<void*> tempStack;
    static int tempStackTop;
    static void PushTemp(void * obj);
    static void PopTemp();
     */
};

#endif //PROTON_VM_H
