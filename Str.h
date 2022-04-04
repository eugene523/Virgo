#ifndef PROTON_STR_H
#define PROTON_STR_H

#include "Obj.h"

struct Str : Obj {
    std::string val;

    static Type * t;
    static void InitType();
    explicit Str(std::string val);
    explicit Str(char val);
    [[nodiscard]] std::string ToStr() const override;
    void Mark() override {}
};

#endif //PROTON_STR_H