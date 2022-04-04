#ifndef VIRGO_OBJECT_H
#define VIRGO_OBJECT_H

#include <map>
#include "Obj.h"
#include "Mem.h"

struct Object : Obj {
    std::map<Ref, Ref> fields;

    static Type * t;
    static void InitType();
    explicit Object();
    void Mark() override;
};

#endif //VIRGO_OBJECT_H