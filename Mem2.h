#ifndef VIRGO_MEM2_H
#define VIRGO_MEM2_H

#include <array>
#include <vector>
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

struct Reference {
    ObjHnd * objHndPtr {};

    Reference() = default;
    explicit Reference(ObjHnd * objHndPtr);
    bool operator==(const Reference & another) const;
    bool operator!=(const Reference & another) const;
    bool operator<(const Reference & another)  const;
    static const Reference none;
};

struct MemDomain {
    static const int MemDomainCapacity = (1 << 8); // 256
    unsigned int generation {};
    std::array<ObjHnd, MemDomainCapacity> handlers {};
    std::array<unsigned int, MemDomainCapacity> freeIndicesStack {};
    int freeIndicesStackTop {};

    MemDomain();
    void Init();
    Ref NewRef();
    void Mark();
    void Sweep();
    void CollectGarbage();
    bool HasFreeHandlers();
};

struct Heap {
    static std::vector<MemDomain*> domains;

    // Temporary references stack is used during evaluating expressions.
    // We push to this stack intermediate results of a currently evaluating expression,
    // so we can be sure that intermediate values will not be deleted if GC will start
    // in the middle of expression.
    static const int RefStackCapacity = (1 << 10); // 1024
    static std::array<Ref, RefStackCapacity> refStack;
    static int refStackTop;

    static void (*MarkCallback) ();
};


#endif //VIRGO_MEM2_H
