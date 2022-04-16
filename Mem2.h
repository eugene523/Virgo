#ifndef VIRGO_MEM2_H
#define VIRGO_MEM2_H

#include <cassert>
#include <array>
#include <vector>
#include <bitset>
#include "Obj.h"

struct MemDom;

// Object handler
struct ObjHnd {
    Obj * objPtr {};
    MemDom * domain {};
    unsigned int numOfOwners {}; // This field holds owners from other domains.
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

    inline void IncOwners() { objHndPtr->numOfOwners++; }

    inline void DecOwners() {
        assert(objHndPtr->numOfOwners > 0);
        objHndPtr->numOfOwners--;
    }

    bool operator==(const Rf & another) const;

    bool operator!=(const Rf & another) const;

    bool operator<(const Rf & another)  const;
};

///////////////////////////////////////////////////////////////////////////////

// Memory domain
struct MemDom {
    static const unsigned int size;
    unsigned int generation {};
    std::vector<ObjHnd> handlers {};
    std::vector<unsigned int> idxStack {};
    int idxStackTop {};

    explicit MemDom();
    Rf NewRef(Obj * obj);
    void Mark();
    void Sweep();
    void PullUp();
    void CollectGarbage();
    inline bool HasFreeHandlers();
};

///////////////////////////////////////////////////////////////////////////////

struct Heap {
    static std::vector<MemDom*> domains;
    static MemDom * activeDomain;

    // Temporary references stack is used during evaluating expressions.
    // We push to this stack intermediate results of a currently evaluating expression,
    // so we can be sure that intermediate values will not be deleted if GC will start
    // in the middle of expression.
    static const int RefStackCapacity = (1 << 10); // 1024
    static std::vector<Rf> refStack;
    static int refStackTop;

    static void MarkTemp();
    static void UnMarkTemp();
    static void (*PreCollectCallback) ();
    static void (*PostCollectCallback) ();
    static Rf NewRef(Obj * objPtr);
};

#endif //VIRGO_MEM2_H
