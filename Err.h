#ifndef PROTON_ERROR_H
#define PROTON_ERROR_H

#include "Obj.h"

struct Err {
    Obj         obj;
    uint        srcLine;
    std::string message;

    static Type * t;
    static void InitType();
    static Err * New(const std::string & message, uint line = 0);
};

#endif //PROTON_ERROR_H
