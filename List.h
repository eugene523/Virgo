#ifndef PROTON_LIST_H
#define PROTON_LIST_H

#include <vector>
#include <sstream>
#include "Obj.h"
#include "Mem.h"

struct List : public Obj {
    std::vector<Ref> elements {};
    std::map<Ref, Ref> fields {};
    static Type * t;
    static void InitType();
    List();
    void Mark() override;
};

#endif //PROTON_LIST_H
