#ifndef PROTON_BOOL_H
#define PROTON_BOOL_H

#include "Obj.h"
#include "Mem.h"

struct Bool {
    Obj obj;
    bool val {false};

    static Type * t;
    static Bool * True;
    static Bool * False;

    static void InitType();

    static inline Bool * New(bool value) {
        return (value) ? Bool::True : Bool::False;
    }
};

#endif //PROTON_BOOL_H
