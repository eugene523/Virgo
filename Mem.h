#ifndef VIRGO_MEM_H
#define VIRGO_MEM_H

#include <cassert>
#include <array>
#include <vector>
#include <bitset>
#include <cstring>
#include "Obj.h"

struct MemDomain;

struct PageHeader {
    MemDomain * domain;
    uint chunkSize;
    std::byte * next;
};

void Page_Init(std::byte * page, MemDomain * domain, uint chunkSize);

std::byte * Page_GetChunk(PageHeader * page);

extern const std::uint64_t PAGE_MASK;

#define GET_PAGE(ptr) (PageHeader*)((std::uintptr_t)ptr & PAGE_MASK)

void Page_FreeChunk(std::byte * chunk);

int Page_GetNumOfFreeChunks(PageHeader * page);

///////////////////////////////////////////////////////////////////////////////

// Object handler
struct ObjHnd {
    Obj * obj {};
    MemDomain * domain {};
    uint numOfOwners {}; // This field holds owners from other domains.
    uint generation  {};
};

///////////////////////////////////////////////////////////////////////////////

// Reference
struct Ref {
    ObjHnd * objHnd {};
    static const Ref none;
    Ref() = default;
    explicit Ref(ObjHnd * objHndPtr);
    inline Obj * GetObj() { return objHnd->obj; }
    inline MemDomain * GetDomain() { return objHnd->domain; }
    inline void IncOwners() { objHnd->numOfOwners++; }
    inline void DecOwners() {
        assert(objHnd->numOfOwners > 0);
        objHnd->numOfOwners--;
    }

    bool operator==(const Ref & another) const;
    bool operator!=(const Ref & another) const;
    bool operator<(const Ref & another)  const;
};

///////////////////////////////////////////////////////////////////////////////

// Memory domain
struct MemDomain {
    bool isConstant {};
    static const unsigned int size;
    uint generation {};
    std::vector<ObjHnd> handlers {};
    std::vector<ObjHnd*> freeHandlersStack {};
    int freeHandlersStackTop {};

    explicit MemDomain();
    Ref NewRef(Obj * obj);
    void FreeRef(Ref ref);
    void Mark();
    void Sweep();
    void CollectGarbage();
    inline uint NumOfObjects() { return size - (freeHandlersStackTop + 1); }
    inline bool HasFreeHandlers();
    void PrintStatus(const std::string & additionalMessage = "");
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
    static const int RefStackCapacity;
    static std::vector<Ref> refStack;
    static int refStackTop;

    static void (*PreCollectCallback) ();
    static void (*PostCollectCallback) ();

    static void Init();
    static void CollectGarbageInDomain(MemDomain * targetDomain);
    static Ref NewRef(Obj * obj);
    static Ref NewRefInDomain(Obj * obj, MemDomain * targetDomain);
    static Ref NewRefNeighbour(Obj * obj, Ref neighbour);
    static Ref NewPreservedRef(Obj * obj);
    static Ref NewConstantRef(Obj * obj);
    static Ref TransferRef(Ref ref, MemDomain * targetDomain);
    static void PushRef(Ref rf);
    static void PopRef();
    static void MarkTemp();
    static void UnmarkTemp();
};

#define GET_OBJ(ref) ref.GetObj()

#define NEW_REF(objPtr) Heap::NewRef(objPtr)

#define NEW_CONSTANT(objPtr) Heap::NewConstantRef(objPtr)

#define PUSH_TEMP(ref) Heap::PushRef(ref)

#define POP_TEMP Heap::PopRef(ref)

#endif //VIRGO_MEM_H
