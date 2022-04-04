#ifndef PROTON_BREAK_H
#define PROTON_BREAK_H

#include "Obj.h"

struct Break : public Obj {
    static Type * t;
    static void InitType();
    Break();
    ~Break() override;
    void Mark() override;
};

#endif //PROTON_BREAK_H
