#ifndef VIRGO_BOOL_H
#define VIRGO_BOOL_H

#include "Obj.h"
#include "Mem.h"

struct Bool : Obj {
    bool val {false};

    explicit Bool(bool value);

    static Type * t;
    static Bool * True;
    static Bool * False;

    static void InitType();
    static void InitConstants();

    static inline Bool * New(bool value) {
        return value ? Bool::True : Bool::False;
    }

    static Obj * Not(Obj * self);
    static Obj * And(Obj * self, Obj * other);
    static Obj * Or(Obj * self, Obj * other);

    inline Bool * Invert() {
        return val ? Bool::False : Bool::True;
    }
};

#endif //VIRGO_BOOL_H
