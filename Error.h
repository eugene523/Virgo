#ifndef PROTON_ERROR_H
#define PROTON_ERROR_H

#include "Obj.h"

struct Error {
    Obj         obj;
    uint        srcLine;
    std::string message;

    static Type * t;
    static void InitType();
    static Error * New(const std::string & message, uint line = 0);
};

#endif //PROTON_ERROR_H
