#include <cassert>
#include <iostream>
#include <iomanip>
#include "Mem.h"
#include "Type.h"

const uint          ALIGNMENT     = 8;
const uint          PAGE_POWER    = 12;
const uint          PAGE_SIZE     = (1u << PAGE_POWER); // 4096
const std::uint64_t PAGE_MASK     = (~std::uint64_t(0) << PAGE_POWER); // 0xFFF
const uint          BLOCK_SIZE    = (1u << 24u); // 16MB
const uint          BLOCK_SIZE_MB = 16;

std::vector<std::byte*> MemBank::blocks;

std::stack<std::byte*> MemBank::freePages;

void MemBank::AllocateBlock() {
    std::byte * block = (std::byte*)calloc(BLOCK_SIZE, 1);
    blocks.push_back(block);

    // Find a position in the block which is aligned according to the page size.
    const std::uint64_t mask = 0xFFF;
    std::byte * nextPage = nullptr;
    int numOfPages = 0;
    std::uint64_t lastBits = (std::uint64_t)block & mask;
    if (lastBits == 0) {
        nextPage = block;
        numOfPages = BLOCK_SIZE/PAGE_SIZE;
    } else {
        nextPage = (std::byte*)((std::uint64_t)(block + PAGE_SIZE) & PAGE_MASK);
        numOfPages = BLOCK_SIZE/PAGE_SIZE - 1;
    }

    // Slicing block into pieces.
    for (int i = 0; i < numOfPages; i++) {
        freePages.push(nextPage);
        nextPage += PAGE_SIZE;
    }
}

std::byte * MemBank::GetPage() {
    if (freePages.empty())
        AllocateBlock();
    std::byte * page = freePages.top();
    freePages.pop();
    return page;
}

void MemBank::AcceptPage(std::byte * page) {
    freePages.push(page);
}

void MemBank::PrintStatus() {
    std::cout << "\nMemBank status:\n";
    std::cout << "blocks    : " << blocks.size() << '\n'
              << "allocated : " << blocks.size() * BLOCK_SIZE_MB << " Mb\n"
              << "freePages : " << freePages.size() << '\n';
}

///////////////////////////////////////////////////////////////////////////////

struct ChunkHeader {
    std::uint64_t zeroField; // Always zero for a free chunk
    std::byte * nextChunk;
};

inline bool IsFreeChunk(void * chunkPtr) {
    return ((ChunkHeader*)chunkPtr)->zeroField == 0;
}

const uint PAGE_AVAILABLE_SPACE = PAGE_SIZE - sizeof(Page);

#define PAGE_CAPACITY(chunkSize) (PAGE_AVAILABLE_SPACE / chunkSize)

void Page::Init(std::byte * pagePtr, MemDomain * domain, uint chunkSize) {
    memset(pagePtr, 0, PAGE_SIZE);

    auto * page = (Page*)pagePtr;
    page->domain = domain;
    page->chunkSize = chunkSize;

    uint capacity = PAGE_CAPACITY(chunkSize);
    std::byte * chunk = pagePtr + sizeof(Page);
    page->nextFreeChunk = chunk;
    for (uint i = 0; i < (capacity - 1); i++) {
        std::byte * nextChunk = chunk + chunkSize;
        ((ChunkHeader*)chunk)->nextChunk = nextChunk;
        chunk = nextChunk;
    }
    ((ChunkHeader*)chunk)->nextChunk = nullptr;
}

std::byte * Page::GetChunk() {
    if (nextFreeChunk == nullptr)
        return nullptr;
    std::byte * chunk = nextFreeChunk;
    nextFreeChunk = ((ChunkHeader*)nextFreeChunk)->nextChunk;
    return chunk;
}

// FreeChunk function doesn't care about the content of a chunk.
// It just pushes chunk to the free list of vacant chunks.
// So, be aware that if some object sits inside this chunk,
// then it must be properly deconstructed before a FreeChunk call.
// We also zeroing out chunk (first 8 bytes must be zero)
// in order to show garbage collector that this is a free chunk and it must be skipped.
void Page::FreeChunk(std::byte * chunk) {
    assert(((ChunkHeader*)chunk)->zeroField != 0);
    Page * page = Page::GetPage(chunk);
    memset(chunk, 0, page->chunkSize);
    ((ChunkHeader*)chunk)->nextChunk = page->nextFreeChunk;
    page->nextFreeChunk = chunk;
}

uint Page::NumOfFreeChunks() {
    uint numOfFreeChunks = 0;
    std::byte * chunk = nextFreeChunk;
    while (chunk != nullptr) {
        numOfFreeChunks++;
        chunk = ((ChunkHeader*)chunk)->nextChunk;
    }
    return numOfFreeChunks;
}

uint Page::NumOfObj() {
    return (PAGE_CAPACITY(chunkSize) - NumOfFreeChunks());
}

bool Page::IsEmpty() {
    return NumOfFreeChunks() == PAGE_CAPACITY(chunkSize);
}

#include "Obj.h"

void Page::Mark() {
    uint capacity = PAGE_CAPACITY(chunkSize);
    std::byte * chunk = (std::byte*)this + sizeof(Page);
    bool currentColor = domain->Get_MarkColor();
    for (uint i = 0; i < capacity; i++, chunk += chunkSize) {
        if (IsFreeChunk(chunk)) {
            continue;
        }

        Obj * obj = (Obj*)chunk;

        // Check if it is already marked.
        if (obj->Get_MarkColor() == currentColor)
            continue;

        if (obj->numOfOwners == 0)
            continue;

        obj->Mark();
    }
}

void Page::Sweep() {
    uint capacity = PAGE_CAPACITY(chunkSize);
    std::byte * chunk = (std::byte*)this + sizeof(Page);
    bool currentColor = domain->Get_MarkColor();
    for (uint i = 0; i < capacity; i++, chunk += chunkSize) {
        if (IsFreeChunk(chunk))
            continue;

        Obj * obj = (Obj*)chunk;

        // If it's marked with proper color then we skip it.
        if (obj->Get_MarkColor() == currentColor)
            continue;

        obj->Delete();
        Page::FreeChunk((std::byte*)obj);
    }
}

///////////////////////////////////////////////////////////////////////////////

void PageCluster::QueryPage() {
    std::byte * page = MemBank::GetPage();
    Page::Init(page, domain, chunkSize);
    pages.push_back((Page*)page);
    activePage = (Page*)page;
}

void PageCluster::UpdateActivePage() {
    activePage = nullptr;
    for (auto * p : pages) {
        if (p->HasFreeChunk()) {
            activePage = p;
            break;
        }
    }

    if (activePage == nullptr)
        QueryPage();
}

void PageCluster::Mark() {
    for (auto * p : pages) {
        p->Mark();
    }
}

void PageCluster::Sweep() {
    for (auto * p : pages) {
        p->Sweep();
    }
}

uint PageCluster::NumOfPages() {
    return pages.size();
}

uint PageCluster::Capacity() {
    return (NumOfPages() * PAGE_CAPACITY(chunkSize));
}

uint PageCluster::NumOfObj() {
    uint numOfObj = 0;
    for (auto * p : pages)
        numOfObj += p->NumOfObj();
    return numOfObj;
}

//void PageCluster::ReleaseEmptyPages() {}

///////////////////////////////////////////////////////////////////////////////

MemDomain::MemDomain() {
    for (std::size_t i = 0; i < clusters.size(); i++) {
        clusters[i].domain = this;
        clusters[i].chunkSize = (i + 3) * ALIGNMENT;
        clusters[i].QueryPage();
    }
}

std::byte * MemDomain::GetChunk(uint chunkSize) {
    assert(chunkSize >= 24);
    assert(chunkSize <= 64);
    uint clusterIndex = 0;
    if (chunkSize % ALIGNMENT == 0) {
        clusterIndex = chunkSize / ALIGNMENT - 3;
    } else {
        // Actually clusterIndex = ((chunkSize / ALIGNMENT) + 1) - 3;
        clusterIndex = (chunkSize / ALIGNMENT) - 2;
    }

    auto & cluster = clusters[clusterIndex];
    if (!cluster.activePage->HasFreeChunk())
        cluster.UpdateActivePage();
    return cluster.activePage->GetChunk();
}

uint MemDomain::NumOfPages() {
    uint numOfPages = 0;
    for (auto & c : clusters)
        numOfPages += c.NumOfPages();
    return numOfPages;
}

uint MemDomain::Capacity() {
    uint capacity = 0;
    for (auto & c : clusters)
        capacity += c.Capacity();
    return capacity;
}

uint MemDomain::NumOfObj() {
    uint numOfObj = 0;
    for (auto & c : clusters)
        numOfObj += c.NumOfObj();
    return numOfObj;
}

void MemDomain::Mark() {
    flags[Bit_MarkColor].flip();
    for (auto & cluster : clusters) {
        cluster.Mark();
    }
}

void MemDomain::Sweep() {
    for (auto & cluster : clusters) {
        cluster.Sweep();
    }
}

void MemDomain::CollectGarbage() {
    Mark();
    Sweep();
}

void MemDomain::PrintStatus(const std::string & additionalMessage /* = "" */) {
    std::cout << '\n' << std::string(60, '-') << '\n';
    std::cout << "MemDomain status";

    if (!additionalMessage.empty())
        std::cout << " [" << additionalMessage << "]:" << "\n";
    else
        std::cout << "\n";

    uint width = 10;
    std::cout << "pages    : " << NumOfPages() << '\n'
              << "capacity : " << Capacity()   << '\n'
              << "objects  : " << NumOfObj()   << '\n'
              << "memory   : " <<

    std::cout << "\nclusters:\n";
    std::cout << std::left
              << std::setw(width) << "chunk_sz"
              << std::setw(width) << "page_cap"
              << std::setw(width) << "pages"
              << std::setw(width) << "full_cap"
              << std::setw(width) << "objects"
              << std::setw(width) << "occupancy"
              << '\n';

    for (auto & c : clusters) {
        uint chunkSize    = c.chunkSize;
        uint pageCapacity = PAGE_CAPACITY(chunkSize);
        uint numOfPages   = c.NumOfPages();
        uint fullCapacity = pageCapacity * numOfPages;
        uint numOfObjects = c.NumOfObj();
        uint occupancy    = (((double)numOfObjects)/fullCapacity) * 100;

        std::cout << std::setw(width) << chunkSize
                  << std::setw(width) << pageCapacity
                  << std::setw(width) << numOfPages
                  << std::setw(width) << fullCapacity
                  << std::setw(width) << numOfObjects
                  << std::setw(width) << (std::to_string(occupancy) + " %")
                  << '\n';
    }
}

///////////////////////////////////////////////////////////////////////////////

std::vector<MemDomain*> Heap::domains;

MemDomain * Heap::constantDomain;
MemDomain * Heap::babyDomain;
MemDomain * Heap::activeDomain;

const uint Heap::TEMP_STACK_CAPACITY{1 << 10}; // 1024
std::list<void*> Heap::tempStack;
int Heap::tempStackTop;

void (*Heap::PreCollectCallback)();
void (*Heap::PostCollectCallback)();

void Heap::Init() {
    tempStack.resize(TEMP_STACK_CAPACITY);
    tempStackTop = -1;

    constantDomain = new MemDomain();
    constantDomain->Set_IsConstant(true);

    babyDomain = new MemDomain();
    babyDomain->Set_IsBaby(true);

    domains.push_back(new MemDomain());
    activeDomain = domains[0];
}

std::byte * Heap::GetChunk(std::size_t chunkSize) {
    return babyDomain->GetChunk(chunkSize);
}

std::byte * Heap::GetChunk_Const(std::size_t chunkSize) {
    return constantDomain->GetChunk(chunkSize);
}

void Heap::PushTemp(void * obj) {
    if (tempStackTop == TEMP_STACK_CAPACITY) {
        std::cerr << "Error. Stack overflow." << std::endl;
        exit(1);
    }
    tempStackTop++;
}

void Heap::PopTemp() {
    assert(tempStackTop >= 0);
    tempStackTop--;
}