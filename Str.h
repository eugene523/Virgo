#ifndef PROTON_STR_H
#define PROTON_STR_H

#include "Obj.h"

struct Str {
    Obj obj;
    std::string val;

    static Type * t;
    static void InitType();
    static Str * New(std::string value);
};

#endif //PROTON_STR_H