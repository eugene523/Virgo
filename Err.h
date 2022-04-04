#ifndef PROTON_ERROR_H
#define PROTON_ERROR_H

#include "Obj.h"

struct Err : Obj {
    uint        line {};
    std::string message;

    static Type * t;
    static void InitType();
    explicit Err(std::string message, uint line = 0);
    [[nodiscard]] std::string ToStr() const override;
    void Mark() override {}
};

#endif //PROTON_ERROR_H
