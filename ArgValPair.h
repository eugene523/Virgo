#ifndef PROTON_ARGVALPAIR_H
#define PROTON_ARGVALPAIR_H

#include <string>
#include "Obj.h"
#include "Mem.h"

struct ArgValPair : public Obj {
    const Ref name;
    const Ref val;

    static Type * t;
    static void InitType();
    ArgValPair(Ref name, Ref val);
    ~ArgValPair() override;
    void Mark() override;
};

#endif //PROTON_ARGVALPAIR_H
