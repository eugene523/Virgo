#ifndef VIRGO_MEM2_H
#define VIRGO_MEM2_H

#include <array>
#include <vector>
#include <bitset>
#include "Obj.h"

struct MemDom;

// Object handler
struct ObjHnd {
    Obj * objPtr {};
    MemDom * domain {};
    unsigned int numOfOwners {};
    unsigned int generation  {};
};

///////////////////////////////////////////////////////////////////////////////

// Reference
struct Rf {
    ObjHnd * objHndPtr {};

    static const Rf none;
    Rf() = default;
    explicit Rf(ObjHnd * objHndPtr);
    inline Obj * GetObj() { return objHndPtr->objPtr; }
    inline MemDom * GetDomain() { return objHndPtr->domain; }
    bool operator==(const Rf & another) const;
    bool operator!=(const Rf & another) const;
    bool operator<(const Rf & another)  const;
};

///////////////////////////////////////////////////////////////////////////////

// Memory domain
struct MemDom {
    static const unsigned int MemDomSize;
    unsigned int generation {};
    std::vector<ObjHnd> handlers {};
    std::vector<unsigned int> freeIndicesStack {};
    int freeIndicesStackTop {};

    explicit MemDom();
    Rf NewRef(Obj * obj);
    void Mark();
    void Sweep();
    void CollectGarbage();
    bool HasFreeHandlers();
};

///////////////////////////////////////////////////////////////////////////////

struct Heap {
    static std::vector<MemDom*> domains;

    // Temporary references stack is used during evaluating expressions.
    // We push to this stack intermediate results of a currently evaluating expression,
    // so we can be sure that intermediate values will not be deleted if GC will start
    // in the middle of expression.
    static const int RefStackCapacity = (1 << 10); // 1024
    static std::vector<Rf> refStack;
    static int refStackTop;

    static void (*MarkCallback) ();
};


#endif //VIRGO_MEM2_H
