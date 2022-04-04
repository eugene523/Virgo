#ifndef PROTON_RETURN_H
#define PROTON_RETURN_H

#include "Mem.h"

struct Return : public Obj {
    Ref retRef;
    static Type * t;
    static void InitType();
    explicit Return(Ref retRef);
    void Mark() override;
};

#endif //PROTON_RETURN_H
