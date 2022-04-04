#ifndef PROTON_REAL_H
#define PROTON_REAL_H

#include "Obj.h"

struct Real : Obj {
    v_real val {};

    static Type * t;
    static void InitType();
    Real();
    explicit Real(v_real value);
    [[nodiscard]] std::string ToStr() const override;
    void Mark() override {}
};

#endif //PROTON_REAL_H
