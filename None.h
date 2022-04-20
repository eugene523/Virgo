#ifndef VIRGO_NONE_H
#define VIRGO_NONE_H

#include "Obj.h"

struct None : Obj {
    static Type * t;
    static void InitType();
    None();
};

#endif //VIRGO_NONE_H