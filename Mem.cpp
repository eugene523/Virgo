#include <cassert>
#include <iostream>
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

const uint NEXT_FIELD_DISPLACEMENT = 8;
const uint PAGE_AVAILABLE_SPACE = PAGE_SIZE - sizeof(Page);

#define PAGE_CAPACITY(chunkSize) (PAGE_AVAILABLE_SPACE / chunkSize)

#define NEXT_FIELD(chunkPtr) ((std::byte**)(chunkPtr + NEXT_FIELD_DISPLACEMENT))

#define IS_FREE_CHUNK(chunkPtr) (*((std::byte**)chunkPtr) == nullptr)

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
        *NEXT_FIELD(chunk) = nextChunk;
        chunk = nextChunk;
    }
    *NEXT_FIELD(chunk) = nullptr;
}

std::byte * Page::GetChunk() {
    if (nextFreeChunk == nullptr)
        return nullptr;
    std::byte * chunk = nextFreeChunk;
    nextFreeChunk = *NEXT_FIELD(nextFreeChunk);
    return chunk;
}

// FreeChunk function doesn't care about the content of a chunk.
// It just pushes chunk to the free list of vacant chunks.
// So, be aware that if some object sits inside this chunk,
// then it must be properly deconstructed before a FreeChunk call.
// We also zeroing out chunk (first 8 bytes must be zero)
// in order to show garbage collector that this is a free chunk and it must be skipped.
void Page::FreeChunk(std::byte * chunk) {
    Page * page = GET_PAGE(chunk);
    memset(chunk, 0, page->chunkSize);
    std::byte * next = page->nextFreeChunk;
    page->nextFreeChunk = chunk;
    *NEXT_FIELD(chunk) = next;
}

uint Page::NumOfFreeChunks() {
    uint numOfFreeChunks = 0;
    std::byte * chunk = nextFreeChunk;
    while (chunk != nullptr) {
        numOfFreeChunks++;
        chunk = *NEXT_FIELD(chunk);
    }
    return numOfFreeChunks;
}

bool Page::IsEmpty() { return NumOfFreeChunks() == PAGE_CAPACITY(chunkSize); }

void Page::Mark() {
    uint availableSpace = PAGE_SIZE - sizeof(Page);
    uint capacity = availableSpace / chunkSize;
    std::byte * chunk = (std::byte*)this + sizeof(Page);
    for (uint i = 0; i < capacity; i++) {
        if (IS_FREE_CHUNK(chunk)) {
            chunk = *NEXT_FIELD(chunk);
            continue;
        }

    }
}

///////////////////////////////////////////////////////////////////////////////

void PageCluster::QueryPage() {
    std::byte * page = MemBank::GetPage();
    Page::Init(page, parentDomain, chunkSize);
    pages.push_back((Page*)page);
    activePage = (Page*)page;
}

void PageCluster::UpdateActivePage() {
    activePage = nullptr;
    for (auto p : pages) {
        if (p->HasFreeChunk()) {
            activePage = p;
            break;
        }
    }

    if (activePage == nullptr)
        QueryPage();
}

///////////////////////////////////////////////////////////////////////////////

const uint MemDomain::PAGE_LIMIT = 1024;

MemDomain::MemDomain() {
    for (int i = 0; i < 6; i++) {
        clusters[i].parentDomain = this;
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

void MemDomain::PrintStatus() {
    std::cout << "\nMemDomain status:\n";
    for (const auto & c : clusters) {
        std::cout << c.chunkSize << ' ' << c.pages.size() << ' ' << c.activePage->NumOfFreeChunks() << '\n';
    }
}

///////////////////////////////////////////////////////////////////////////////

std::vector<MemDomain*> Heap::domains;

MemDomain * Heap::constantDomain;
MemDomain * Heap::babyDomain;
MemDomain * Heap::activeDomain;

const uint Heap::TEMP_STACK_CAPACITY{1 << 10}; // 1024
std::list<Obj*> Heap::tempStack;
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

void Heap::PushTemp(Obj * obj) {
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