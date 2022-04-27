#ifndef VIRGO_NONE_H
#define VIRGO_NONE_H

#include "Obj.h"

struct None {
    Obj obj;
    std::uint64_t fakeField = 0; // We use it just to make None of 24-byte size;

    static Type * t;
    static None * constant;
    static void InitType();
    static inline None * New() { return constant; }
};

#endif //VIRGO_NONE_H