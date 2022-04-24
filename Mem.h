#ifndef VIRGO_MEM_H
#define VIRGO_MEM_H

#include <cassert>
#include <cstring>
#include <array>
#include <vector>
#include <list>
#include <stack>
#include <bitset>
#include "Obj.h"

extern const unsigned int  PAGE_SIZE;
extern const std::uint64_t PAGE_MASK;
extern const unsigned int  ALIGNMENT;

struct MemBank {
    static std::vector<std::byte*> blocks;
    static std::stack<std::byte*> freePages;

    static void AllocateBlock();
    static std::byte * GetPage();
    static void AcceptPage(std::byte * page);
};

///////////////////////////////////////////////////////////////////////////////

struct MemDomain;

struct Page {
    MemDomain * domain;
    unsigned int chunkSize;
    std::byte * next;

    static void Init(std::byte * pagePtr, MemDomain * domain, unsigned int chunkSize);
    std::byte * GetChunk();
    static void FreeChunk(std::byte * chunk);
    unsigned int NumOfFreeChunks();
    inline bool HasFreeChunk() { return next != nullptr; }
};

std::byte * Page_GetChunk(Page * page);

#define GET_PAGE(ptr) (Page*)((std::uint64_t)ptr & PAGE_MASK)

void Page_FreeChunk(std::byte * chunk);

int Page_GetNumOfFreeChunks(Page * page);

///////////////////////////////////////////////////////////////////////////////

struct PageCluster {
    MemDomain *  parentDomain{};
    unsigned int chunkSize{};
    std::list<Page*> pages{};
    Page * activePage{};
    void QueryPage();
    void UpdateActivePage();
};

struct MemDomain {
    // Every cluster has a chunkSize:
    // chunkSize = (index + 3) * ALIGNMENT
    // i.e. 24, 32, ..., 64
    std::array<PageCluster, 6> clusters{};
    explicit MemDomain();

    [[nodiscard]]
    std::byte * GetChunk(unsigned int chunkSize);

    void PrintStatus();
};

// Object handler
struct ObjHnd {
    Obj * obj {};
    MemDomain * domain {};
    uint numOfOwners {}; // This field holds owners from other domains.
    uint generation  {};
};

///////////////////////////////////////////////////////////////////////////////
/*
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
*/
#endif //VIRGO_MEM_H
