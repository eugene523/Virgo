#ifndef PROTON_REAL_H
#define PROTON_REAL_H

#include "Obj.h"

struct Real {
    Obj obj;
    v_real val {};

    static Type * t;
    static void InitType();
    static void New(void * inPlace, v_real value);
    static Real * New(v_real value);
};

#endif //PROTON_REAL_H
