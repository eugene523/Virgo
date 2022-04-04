#ifndef PROTON_ARGLIST_H
#define PROTON_ARGLIST_H

#include <vector>
#include "Obj.h"
#include "Mem.h"

class ArgList : public Obj {
    std::vector<Ref> args;
public:
    static Type * t;
    static void InitType();
    ArgList();
    ~ArgList() override;
    void Add(Ref arg);
    Ref Get(uint index);
    uint NumOfArguments();
    void Mark() override;
};

#endif //PROTON_ARGLIST_H
