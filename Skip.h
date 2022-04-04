#ifndef PROTON_SKIP_H
#define PROTON_SKIP_H

#include "Obj.h"

struct Skip : public Obj {
    static Type * t;
    static void InitType();
    Skip();
    void Mark() override;
};

#endif //PROTON_SKIP_H
