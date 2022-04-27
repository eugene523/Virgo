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
    static std::stack<std::byte*>  freePages;

    static void AllocateBlock();
    static std::byte * GetPage();
    static void AcceptPage(std::byte * page);
    static void PrintStatus();
};

///////////////////////////////////////////////////////////////////////////////

struct MemDomain;

struct Page {
    MemDomain *  domain;
    unsigned int chunkSize;
    std::byte *  next; // Next free chunk

    static void Init(std::byte * pagePtr, MemDomain * domain, unsigned int chunkSize);
    std::byte * GetChunk();
    static void FreeChunk(std::byte * chunk);
    unsigned int NumOfFreeChunks();
    inline bool HasFreeChunk() { return next != nullptr; }
};

#define GET_PAGE(ptr) ((Page*)((std::uint64_t)ptr & PAGE_MASK))

#define IS_BABY(ptr) (GET_PAGE(ptr)->domain->isBaby)

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
    static const unsigned int PAGE_LIMIT;

    bool isConstant{};
    bool isBaby{};
    bool isFull{};
    unsigned int generation{};

    // Each cluster contains pages of the same chunkSize:
    // chunkSize = (index + 3) * ALIGNMENT
    // i.e. 24, 32, ..., 64
    std::array<PageCluster, 6> clusters{};

    explicit MemDomain();

    [[nodiscard]]
    std::byte * GetChunk(unsigned int chunkSize);

    // TODO:
    // Mark, Sweep, GC

    void PrintStatus();
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
    static const unsigned int TEMP_STACK_CAPACITY;
    static std::list<Obj*> tempStack;
    static int tempStackTop;

    static void (*PreCollectCallback)();
    static void (*PostCollectCallback)();

    static void Init();
    static std::byte * GetChunk(std::size_t chunkSize);
    static std::byte * GetChunk_Const(std::size_t chunkSize);
    static void PushTemp(Obj * obj);
    static void PopTemp();
    /*
    static void CollectGarbageInDomain(MemDomain * targetDomain);
    static Ref NewRef(Obj * obj);
    static Ref NewRefInDomain(Obj * obj, MemDomain * targetDomain);
    static Ref NewRefNeighbour(Obj * obj, Ref neighbour);
    static Ref NewPreservedRef(Obj * obj);
    static Ref NewConstantRef(Obj * obj);
    static Ref TransferRef(Ref ref, MemDomain * targetDomain);

    static void MarkTemp();
    static void UnmarkTemp();
    */
};

#endif //VIRGO_MEM_H
