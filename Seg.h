#ifndef VIRGO_SEG_H
#define VIRGO_SEG_H

#include "Mem.h"

struct Seg : Obj {
    Ref a;
    Ref b;
    Ref step;

    static Type * t;
    static void InitType();
    explicit Seg(Ref a, Ref b, Ref step);
    Ref ToList();

    [[nodiscard]]
    std::string ToStr() const override;

    void Mark() override;
};

#endif //VIRGO_SEG_H
