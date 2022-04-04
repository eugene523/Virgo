#ifndef VIRGO_ITER_H
#define VIRGO_ITER_H

#include "Obj.h"
#include "Mem.h"

struct Iter : Obj {
    Ref target {};
    uint currentIndex {};
    bool IsAvailable();
    bool HasNext();

    [[nodiscard]]
    Ref Next();

    static Type * t;
    static void InitType();
    explicit Iter(Ref target);
    void Mark() override;
};

#endif //VIRGO_ITER_H
