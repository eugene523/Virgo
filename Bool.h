#ifndef PROTON_BOOL_H
#define PROTON_BOOL_H

#include "Obj.h"
#include "Mem.h"

struct Bool : Obj {
    bool val {false};

    static Type * t;
    static void InitType();
    static Ref True;
    static Ref False;
    Bool();
    explicit Bool(bool val);
};

#define BOOL_REF(val) ((val) ? Bool::True : Bool::False)

#endif //PROTON_BOOL_H
