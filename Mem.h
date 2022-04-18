#ifndef VIRGO_MEM_H
#define VIRGO_MEM_H

#include <cassert>
#include <array>
#include <vector>
#include <bitset>
#include "Obj.h"

struct MemDomain;

// Object handler
struct ObjHnd {
    Obj * objPtr {};
    MemDomain * domain {};
    unsigned int numOfOwners {}; // This field holds owners from other domains.
    unsigned int generation  {};
};

///////////////////////////////////////////////////////////////////////////////

// Reference
struct Ref {
    ObjHnd * objHndPtr {};
    static const Ref none;
    Ref() = default;
    explicit Ref(ObjHnd * objHndPtr);
    inline Obj * GetObj() { return objHndPtr->objPtr; }
    inline MemDomain * GetDomain() { return objHndPtr->domain; }
    inline void IncOwners() { objHndPtr->numOfOwners++; }
    inline void DecOwners() {
        assert(objHndPtr->numOfOwners > 0);
        objHndPtr->numOfOwners--;
    }

    bool operator==(const Ref & another) const;
    bool operator!=(const Ref & another) const;
    bool operator<(const Ref & another)  const;
};

///////////////////////////////////////////////////////////////////////////////

// Memory domain
struct MemDomain {
    bool isConstantDomain {};
    bool isActiveDomain {};
    static const unsigned int size;
    unsigned int generation {};
    std::vector<ObjHnd> handlers {};
    std::vector<ObjHnd*> freeHandlersStack {};
    int freeHandlersStackTop {};

    explicit MemDomain();
    Ref NewRef(Obj * obj);
    void FreeRef(Ref ref);
    void Mark();
    void Sweep();
    void CollectGarbage();
    inline bool HasFreeHandlers();
};

///////////////////////////////////////////////////////////////////////////////

struct Heap {
    static std::vector<MemDomain*> domains;
    static MemDomain * constantDomain;
    static MemDomain * babyDomain;
    static MemDomain * activeDomain;

    // Temporary references stack is used during evaluating expressions.
    // We push to this stack intermediate results of a currently evaluating expression,
    // so we can be sure that intermediate values will not be deleted if GC will start
    // in the middle of expression.
    static const int RefStackCapacity = (1 << 10); // 1024
    static std::vector<Ref> refStack;
    static int refStackTop;

    static void Init();
    static Ref NewRef(Obj * objPtr);
    static Ref NewPreservedRef(Obj * objPtr);
    static void PushRef(Ref rf);
    static void PopRef();
    static void MarkTemp();
    static void UnmarkTemp();
    static void (*PreCollectCallback) ();
    static void (*PostCollectCallback) ();
};

#define GET_OBJ(ref) ref.GetObj()

#define NEW_REF(objPtr) Heap::NewRef(objPtr)

#define NEW_PRESERVED_REF(obj) Mem::NewPreservedRef(obj)

#define PUSH_TEMP(ref) Heap::PushRef(ref)

#define POP_TEMP Heap::PopRef(ref)

#endif //VIRGO_MEM_H
