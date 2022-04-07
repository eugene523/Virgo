#ifndef VIRGO_MEM2_H
#define VIRGO_MEM2_H

#include <bitset>
#include "Obj.h"

const int ObjHnd_IsPreserved = 0;
const int ObjHnd_IsOnStack   = 1;

struct ObjHnd {
    Obj * objPtr {};
    unsigned int generation {};
    unsigned int numOfExternalOwners {};
    unsigned int domainId {};
    std::bitset<32> flags {};
};

struct Ref2 {
    ObjHnd * objHndPtr {};

    Ref2() = default;
    explicit Ref2(ObjHnd * objHndPtr);
    bool operator==(const Ref & another) const;
    bool operator!=(const Ref & another) const;
    bool operator<(const Ref & another) const;
    static const Ref2 none;
};

const int MemDomainCapacity = 1024;

struct MemDomain {
    ObjHnd handlers[MemDomainCapacity];
    unsigned int freeIndicesStack[MemDomainCapacity];
};

#endif //VIRGO_MEM2_H
