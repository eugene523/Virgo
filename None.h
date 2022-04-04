#ifndef VIRGO_NONE_H
#define VIRGO_NONE_H

#include "Obj.h"

struct None : Obj {
    static Type * t;
    static void InitType();
    None();
    [[nodiscard]] std::string ToStr() const override;
    void Mark() override {}
};

#endif //VIRGO_NONE_H