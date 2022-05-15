#ifndef PROTON_INT_H
#define PROTON_INT_H

#include "Obj.h"

struct Int {
    Obj obj;
    v_int val {};

    static Type * t;
    static void InitType();
    static void New(void * inPlace, v_int value);
    static Int * New(v_int value);
};

#endif //PROTON_INT_H
