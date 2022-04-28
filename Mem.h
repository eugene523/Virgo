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

extern const uint          PAGE_SIZE;
extern const std::uint64_t PAGE_MASK;
extern const uint          ALIGNMENT;

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
    MemDomain * domain;
    uint        chunkSize;
    std::byte * nextFreeChunk;

    static void Init(std::byte * pagePtr, MemDomain * domain, uint chunkSize);
    std::byte * GetChunk();
    static void FreeChunk(std::byte * chunk);
    uint NumOfFreeChunks();
    inline bool HasFreeChunk() { return nextFreeChunk != nullptr; }
    bool IsEmpty();
    void Mark();
};

#define GET_PAGE(chunkPtr) ((Page*)((std::uint64_t)chunkPtr & PAGE_MASK))

#define IS_BABY(ptr) (GET_PAGE(ptr)->domain->flagBits[MD_IS_BABY])

///////////////////////////////////////////////////////////////////////////////

struct PageCluster {
    MemDomain *  parentDomain{};
    uint chunkSize{};
    std::list<Page*> pages{};
    Page * activePage{};
    void QueryPage();
    void UpdateActivePage();
};

struct MemDomain {
    static const uint PAGE_LIMIT;

    enum
    {
        MemDomain_MarkColor_Bit,
        MemDomain_IsConstant_Bit,
        MemDomain_IsBaby_Bit,
    };
    std::bitset<32> flagBits{};

    uint gcGeneration{};

    // Each cluster contains pages of the same chunkSize:
    // chunkSize = (index + 3) * ALIGNMENT
    // i.e. 24, 32, ..., 64
    std::array<PageCluster, 6> clusters{};

    explicit MemDomain();

    inline bool Get_IsBaby() { return flagBits[MemDomain_IsBaby_Bit]; }
    inline void Set_IsBaby(bool value) { flagBits[MemDomain_IsBaby_Bit] = value; }

    inline bool Get_IsConstant() { return flagBits[MemDomain_IsConstant_Bit]; }
    inline void Set_IsConstant(bool value) { flagBits[MemDomain_IsConstant_Bit] = value; }

    [[nodiscard]]
    std::byte * GetChunk(uint chunkSize);

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
    static const uint TEMP_STACK_CAPACITY;
    static std::list<Obj*> tempStack;
    static int tempStackTop;

    static void (*PreCollectCallback)();
    static void (*PostCollectCallback)();

    static void Init();
    static std::byte * GetChunk(std::size_t chunkSize);
    static std::byte * GetChunk_Const(std::size_t chunkSize);
    static void PushTemp(Obj * obj);
    static void PopTemp();
};

#endif //VIRGO_MEM_H
