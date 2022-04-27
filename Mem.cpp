#include <cassert>
#include <iostream>
#include <sstream>
#include "Mem.h"
#include "Type.h"

const unsigned int  PAGE_SIZE = 4096;
const std::uint64_t PAGE_MASK = ~0xFFFull;
const unsigned int  ALIGNMENT = 8;
const unsigned int  BLOCK_SIZE = (1 << 24); // 16MB
const unsigned int  BLOCK_SIZE_MB = 16;

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

    // Slice block into pieces.
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

void Page::Init(std::byte * pagePtr, MemDomain * domain, unsigned int chunkSize) {
    memset(pagePtr, 0, PAGE_SIZE);

    auto * page = (Page*)pagePtr;
    page->domain = domain;
    page->chunkSize = chunkSize;

    int availableSpace = PAGE_SIZE - sizeof(Page);
    int numOfChunks = availableSpace / chunkSize;
    std::byte * next = pagePtr + sizeof(Page);
    page->next = next;
    for (int i = 0; i < (numOfChunks - 1); i++) {
        *((std::byte**)next) = next + chunkSize;
        next += chunkSize;
    }
    *((std::byte**)next) = nullptr;
}

std::byte * Page::GetChunk() {
    if (next == nullptr)
        return nullptr;
    std::byte * chunk = next;
    next = *((std::byte**)next);
    return chunk;
}

void Page::FreeChunk(std::byte * chunk) {
    Page * page = GET_PAGE(chunk);
    std::byte * next = page->next;
    page->next = chunk;
    *((std::byte**)chunk) = next;
}

unsigned int Page::NumOfFreeChunks() {
    unsigned int numOfFreeChunks = 0;
    std::byte * nextPtr = next;
    while (nextPtr != nullptr) {
        numOfFreeChunks++;
        nextPtr = *((std::byte**)nextPtr);
    }
    return numOfFreeChunks;
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

const unsigned int MemDomain::PAGE_LIMIT = 1024;

MemDomain::MemDomain() {
    for (int i = 0; i < 6; i++) {
        clusters[i].parentDomain = this;
        clusters[i].chunkSize = (i + 3) * ALIGNMENT;
        clusters[i].QueryPage();
    }
}

std::byte * MemDomain::GetChunk(unsigned int chunkSize) {
    assert(chunkSize >= 24);
    assert(chunkSize <= 64);
    unsigned int clusterIndex = 0;
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

const unsigned int Heap::TEMP_STACK_CAPACITY{1 << 10}; // 1024
std::list<Obj*> Heap::tempStack;
int Heap::tempStackTop;

void (*Heap::PreCollectCallback)();
void (*Heap::PostCollectCallback)();

void Heap::Init() {
    tempStack.resize(TEMP_STACK_CAPACITY);
    tempStackTop = -1;

    constantDomain = new MemDomain();
    constantDomain->isConstant = true;

    babyDomain = new MemDomain();
    babyDomain->isBaby = true;

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

/*
void Heap::CollectGarbageInDomain(MemDomain * targetDomain) {
    MarkTemp();
    PreCollectCallback();
    targetDomain->CollectGarbage();
    PostCollectCallback();
    UnmarkTemp();
}

Ref Heap::NewRef(Obj * obj) {
    if (activeDomain->HasFreeHandlers())
        return activeDomain->NewRef(obj);

    CollectGarbageInDomain(activeDomain);

    if (activeDomain->HasFreeHandlers())
        return activeDomain->NewRef(obj);

    auto * memDom = new MemDomain();
    domains.push_back(memDom);
    activeDomain = memDom;
    return memDom->NewRef(obj);
}

Ref Heap::NewRefInDomain(Obj * obj, MemDomain * targetDomain) {
    if (targetDomain->HasFreeHandlers())
        return targetDomain->NewRef(obj);

    CollectGarbageInDomain(targetDomain);

    if (targetDomain->HasFreeHandlers())
        return targetDomain->NewRef(obj);

    return NewRef(obj);
}

Ref Heap::NewRefNeighbour(Obj * obj, Ref neighbour) {
    return NewRefInDomain(obj, neighbour.GetDomain());
}

Ref Heap::NewPreservedRef(Obj * obj) {
    Ref ref = NewRef(obj);
    ref.IncOwners();
    return ref;
}

Ref Heap::NewConstantRef(Obj* obj) {
    assert(constantDomain->HasFreeHandlers());
    return constantDomain->NewRef(obj);
}

Ref Heap::TransferRef(Ref ref, MemDomain * targetDomain) {
    if (ref.GetDomain()->isConstant) {
        ref.IncOwners();
        return ref;
    }
    assert(ref.GetDomain() == babyDomain);
    Obj * objPtr = ref.GetObj();
    babyDomain->FreeRef(ref);
    return NewRefInDomain(objPtr, targetDomain);
}

void Heap::MarkTemp() {
    for (int i = 0; i <= refStackTop; i++) {
        if (refStack[i].GetDomain() != activeDomain)
            continue;
        refStack[i].IncOwners();
    }
}

void Heap::UnmarkTemp() {
    for (int i = 0; i <= refStackTop; i++) {
        if (refStack[i].GetDomain() != activeDomain)
            continue;
        refStack[i].DecOwners();
    }
}
*/