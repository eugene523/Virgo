#ifndef PROTON_NATFUN_H
#define PROTON_NATFUN_H

#include "Obj.h"
#include "Mem.h"

struct NFun : Obj {
    const std::string name;

    static Type * t;
    static void InitType();
    explicit NFun(std::string name);
    ~NFun() override;
    virtual Ref Execute(Ref argList) = 0;
    void Mark() override;

    [[nodiscard]]
    std::string ToStr() const override;
};

#endif //PROTON_NATFUN_H
