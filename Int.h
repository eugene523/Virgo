#ifndef PROTON_INT_H
#define PROTON_INT_H

#include "Obj.h"

struct Int : Obj {
    v_int val {};

    static Type * t;
    static void InitType();
    Int();
    explicit Int(v_int value);

    [[nodiscard]]
    std::string ToStr() const override;

    void Mark() override {}
};

#endif //PROTON_INT_H
