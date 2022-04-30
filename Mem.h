#ifndef VIRGO_MEM_H
#define VIRGO_MEM_H

#include <cassert>
#include <cstring>
#include <array>
#include <vector>
#include <list>
#include <stack>
#include <bitset>
#include "Common.h"

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

    static inline Page * GetPage(void * chunkPtr) {
        return (Page*)((std::uint64_t)chunkPtr & PAGE_MASK);
    }

    std::byte * GetChunk();
    static void FreeChunk(std::byte * chunk);
    uint NumOfFreeChunks();

    inline bool HasFreeChunk() {
        return nextFreeChunk != nullptr;
    }

    bool IsEmpty();
    void Mark();
    void Sweep();
};

#define IS_BABY(ptr) (GET_PAGE(ptr)->domain->flagBits[MD_IS_BABY])

///////////////////////////////////////////////////////////////////////////////

struct PageCluster {
    MemDomain *  domain{};
    uint chunkSize{};
    std::list<Page*> pages{};
    Page * activePage{};

    void QueryPage();
    void UpdateActivePage();
    void Mark();
    void Sweep();
    void ReleaseEmptyPages();
};

struct MemDomain {
    uint pageLimit = 1024;

    enum
    {
        Bit_MarkColor,
        Bit_IsConstant,
        Bit_IsBaby,
    };
    std::bitset<32> flags{};

    uint gcGeneration{};

    // Each cluster contains pages of the same chunkSize:
    // chunkSize = (index + 3) * ALIGNMENT
    // i.e. 24, 32, ..., 64
    std::array<PageCluster, 6> clusters{};

    explicit MemDomain();

    inline bool Get_MarkColor() {
        return flags[Bit_MarkColor];
    }

    /////////////////////////////////////////////

    inline bool Get_IsBaby() {
        return flags[Bit_IsBaby];
    }

    inline void Set_IsBaby(bool value) {
        flags[Bit_IsBaby] = value;
    }

    /////////////////////////////////////////////

    inline bool Get_IsConstant() {
        return flags[Bit_IsConstant];
    }

    inline void Set_IsConstant(bool value) {
        flags[Bit_IsConstant] = value;
    }

    /////////////////////////////////////////////

    void Mark();
    void Sweep();
    void CollectGarbage();
    void ReleaseEmptyPages();

    /////////////////////////////////////////////

    [[nodiscard]]
    std::byte * GetChunk(uint chunkSize);

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
    static std::list<void*> tempStack;
    static int tempStackTop;

    static void (*PreCollectCallback)();
    static void (*PostCollectCallback)();

    static void Init();
    static std::byte * GetChunk(std::size_t chunkSize);
    static std::byte * GetChunk_Const(std::size_t chunkSize);
    static void PushTemp(void * obj);
    static void PopTemp();
};

#endif //VIRGO_MEM_H
