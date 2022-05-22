#ifndef PROTON_STR_H
#define PROTON_STR_H

#include "Obj.h"

struct Str {
    Obj obj;
    const char * val {};
    uint len {};

    static Type * t;
    static void InitType();
    static void New(void * inPlace, const char * value);
    static Str * New(const char * value);
};

#endif //PROTON_STR_H