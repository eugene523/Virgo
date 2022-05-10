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

    static void Init(std::byte * pagePtr,
                     MemDomain * domain,
                     uint        chunkSize);

    static inline Page * GetPage(void * chunkPtr) {
        return (Page*)((std::uint64_t)chunkPtr & PAGE_MASK);
    }

    std::byte * GetChunk();
    static void FreeChunk(std::byte * chunk);
    uint NumOfFreeChunks();
    uint NumOfObj();

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
    MemDomain *        domain{};
    uint               chunkSize{};
    std::vector<Page*> pages{};
    uint               activePageIndex{};
    Page *             activePage{};

    std::byte * GetChunk();
    uint NumOfPages();
    uint Capacity();
    uint NumOfObj();
    void QueryPage();
    void UpdateActivePage();
    void UpdateActivePage_AfterGc();
    void Mark();
    void Sweep();
    void ReleaseEmptyPages();
};

///////////////////////////////////////////////////////////////////////////////

struct MemDomain {
    uint pageLimit = 1024;
    uint totalNumOfPages{};

    uint   lastMarked{};
    uint   lastDeleted{};
    double shrinkFactor{};

    enum
    {
        Flag_IsAvailable,
        Flag_IsConstantDomain,
        Flag_IsBabyDomain,
    };
    std::bitset<32> flags{};

    // Each cluster contains pages that contain chunks of the same size:
    // chunkSize = (index + 3) * ALIGNMENT
    // i.e. 24, 32, ..., 64
    std::array<PageCluster, 6> clusters{};

    explicit MemDomain();

    /////////////////////////////////////////////

    inline bool GetFlag_IsAvailable() { return flags[Flag_IsAvailable]; }
    inline void SetFlag_IsAvailable(bool value) { flags[Flag_IsAvailable] = value; }

    inline bool GetFlag_IsConstant() { return flags[Flag_IsConstantDomain]; }
    inline void SetFlag_IsConstant(bool value) { flags[Flag_IsConstantDomain] = value; }

    inline bool GetFlag_IsBabyDomain() { return flags[Flag_IsBabyDomain]; }
    inline void SetFlag_IsBabyDomain(bool value) { flags[Flag_IsBabyDomain] = value; }

    /////////////////////////////////////////////

    uint NumOfPages();
    uint Capacity();
    uint NumOfObj();
    void Gc();

    /////////////////////////////////////////////

    [[nodiscard]]
    std::byte * GetChunk(uint chunkSize);

    void PrintStatus(const std::string & additionalMessage = "");
};

///////////////////////////////////////////////////////////////////////////////

struct Heap {
    static MemDomain * constantDomain;
    static MemDomain * babyDomain;
    static uint        domainLimit;
    static uint        activeDomainIndex;
    static MemDomain * activeDomain;
    static std::vector<MemDomain*> domains;

    static void (*PreGc)(MemDomain * gcDomain);

    static void Init();
    static std::byte * GetChunk_Constant(std::size_t chunkSize);
    static std::byte * GetChunk_Baby(std::size_t chunkSize);
    static std::byte * GetChunk_Preferable(MemDomain * preferableDomain, std::size_t chunkSize);
    static std::byte * GetChunk_Active(std::size_t chunkSize);
    static void UpdateActiveDomain();

    static void GlobalGc();
    static void UpdateActiveDomain_AfterGlobalGc();
};

#endif //VIRGO_MEM_H
