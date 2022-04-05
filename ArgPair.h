#ifndef PROTON_ARGVALPAIR_H
#define PROTON_ARGVALPAIR_H

#include <string>
#include "Obj.h"
#include "Mem.h"

struct ArgPair : public Obj {
    const Ref name;
    const Ref val;

    static Type * t;
    static void InitType();
    ArgPair(Ref name, Ref val);
    ~ArgPair() override;
    void Mark() override;
};

#endif //PROTON_ARGVALPAIR_H
