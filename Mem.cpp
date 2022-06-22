#include <cassert>
#include <iostream>
#include <iomanip>
#include <climits>
#include "Mem.h"
#include "Type.h"
#include "Utils.h"

/* Virgo automatic memory managment system.
 *
 * Terms:
 *
 * MEMORY BANK - an object that allocate memory blocks and keep stack of
 * free pages.
 *
 *
 * BLOCK - it's just a block of memory of quite significant size (ex. 16 Mb).
 * But actually the size is arbitrary. After memory bank allocates a block,
 * it slices it into pages, and put them on the stack of free pages.
 *
 *
 * PAGE - a block of memory, which usually has the size of 4096 = 2^12 bytes,
 * but in current implementation it can be of any 2^n size. Pages of 2^n size start at specific
 * memory address which is divisible by 2^n. This fact is very important,
 * because if we have some pointer that points to some byte in this page, then we
 * can easily compute start of the page by applying page mask:
 *
 *     page_address = byte_address & page_mask.
 *
 * Each page contains a header placed on the start of the page,
 * that contains some system info. Page maintains a 'free list' of chunks
 * of the same size. See wikipedia on what is 'free list'.
 * In our codebase we also use the term 'page' to denote a page header.
 *
 *
 * CHUNK - a little memory block (24..64 bytes) that lies inside a page.
 * As we mentioned before, each page contains a header and an array of chunks of the same size, that
 * starts right after a header and continues to the end. Of course at the end of the
 * page there will be some unused little area, because the available space of the page
 * after a header is not supposed to be divisible by chunk size. Each chunk
 * is a place where an object live.
 *
 *
 * PAGE CLUSTER - a set of pages. All pages in the cluster maintains chunks of the same size.
 * Page cluster takes and returns free pages to the memory bank.
 *
 *
 * MEMORY DOMAIN - a set of page clusters. Memory domain performs garbage collection,
 * that is scoped to memory which belongs to this particular memory domain.
 *
 *
 * HEAP - a set of memory domains.
 */

///////////////////////////////////////////////////////////////////////////////

const uint          ALIGNMENT     = 8;
const uint          PAGE_POWER    = 12;
const uint          PAGE_SIZE     = (1u << PAGE_POWER); // 4096
const std::uint64_t PAGE_MASK     = (~std::uint64_t(0) << PAGE_POWER); // 0xFF'FF'FF'FF'FF'FF'F0'00
const uint          BLOCK_SIZE    = (1u << 24u); // 16MB
const uint          BLOCK_SIZE_MB = 16;

///////////////////////////////////////////////////////////////////////////////

std::vector<std::byte*> MemBank::blocks;

std::stack<std::byte*> MemBank::freePages;

void MemBank::AllocateBlock() {
    auto * block = (std::byte*)calloc(BLOCK_SIZE, 1);
    if (block == nullptr) {
        std::cerr << "Error. Can't allocate block.";
        abort();
    }
    blocks.push_back(block);

    // Find a position in the block which is aligned according to the page size.
    std::byte * nextPage = nullptr;
    int numOfPages = 0;
    std::uint64_t lastBits = ((std::uint64_t)block) & (~PAGE_MASK);
    if (lastBits == 0) {
        nextPage = block;
        numOfPages = BLOCK_SIZE / PAGE_SIZE;
    } else {
        nextPage = (std::byte*)((std::uint64_t)(block + PAGE_SIZE) & PAGE_MASK);
        numOfPages = (BLOCK_SIZE / PAGE_SIZE) - 1;
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
    std::byte *   nextChunk;
    std::uint64_t zeroField; // Always zero for a free chunk
};

inline bool IsFreeChunk(void * chunkPtr) {
    return ((ChunkHeader*)chunkPtr)->zeroField == 0;
}

const uint PAGE_AVAILABLE_SPACE = PAGE_SIZE - sizeof(Page);

#define PAGE_CAPACITY(chunkSize) (PAGE_AVAILABLE_SPACE / chunkSize)

void Page::Init(std::byte * pagePtr,
                MemDomain * domain,
                uint        chunkSize)
{
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
    for (uint i = 0; i < capacity; i++, chunk += chunkSize) {
        if (IsFreeChunk(chunk))
            continue;

        Obj * obj = (Obj*)chunk;

        // Check if it is already marked.
        if (obj->GetFlag_IsMarked())
            continue;

        if (obj->numOfOwners == 0)
            continue;

        obj->Mark();
    }
}

void Page::Sweep() {
    uint capacity = PAGE_CAPACITY(chunkSize);
    std::byte * chunk = (std::byte*)this + sizeof(Page);
    for (uint i = 0; i < capacity; i++, chunk += chunkSize) {
        if (IsFreeChunk(chunk))
            continue;

        Obj * obj = (Obj*)chunk;

        if (obj->GetFlag_IsMarked()) {
            domain->lastMarked++;
            obj->ResetFlag_IsMarked();
            continue;
        }

        obj->Delete();
        FreeChunk((std::byte*)obj);
        domain->lastDeleted++;
    }
}

void Test_Page() {
    std::byte * pagePtr = MemBank::GetPage();
    Page::Init(pagePtr, nullptr, 32);
    Page * p = (Page*)pagePtr;

    assert(p->IsEmpty());
    assert(p->NumOfObj() == 0);
    assert(p->HasFreeChunk());

    uint nFree = p->NumOfFreeChunks();

    auto * c0 = p->GetChunk();
    assert(!p->IsEmpty());
    assert(p->NumOfFreeChunks() == (nFree - 1));
    assert(p->NumOfObj() == 1);
    Page::FreeChunk(c0);
    assert(p->IsEmpty());
    assert(p->NumOfFreeChunks() == nFree);
    assert(p->NumOfObj() == 0);

    auto * c1 = p->GetChunk();
    auto * c2 = p->GetChunk();
    auto * c3 = p->GetChunk();
    assert(!p->IsEmpty());
    assert(p->NumOfFreeChunks() == (nFree - 3));
    assert(p->NumOfObj() == 3);

    Page::FreeChunk(c1);
    assert(!p->IsEmpty());
    assert(p->NumOfFreeChunks() == (nFree - 2));
    assert(p->NumOfObj() == 2);

    Page::FreeChunk(c2);
    assert(!p->IsEmpty());

    assert(p->NumOfFreeChunks() == (nFree - 1));
    assert(p->NumOfObj() == 1);

    Page::FreeChunk(c3);
    assert(p->IsEmpty());
    assert(p->NumOfFreeChunks() == nFree);
    assert(p->NumOfObj() == 0);

    for (uint i = 0; i < nFree; i++) {
        auto * c = p->GetChunk();
    }
    assert(!p->IsEmpty());
    assert(p->NumOfObj() == nFree);
    assert(!p->HasFreeChunk());

    MemBank::AcceptPage(pagePtr);
}

///////////////////////////////////////////////////////////////////////////////

void PageCluster::Init() {
    QueryPage();
    UpdateActivePage();
}

std::byte * PageCluster::GetChunk() {
    std::byte * chunk = activePage->GetChunk();
    if (chunk != nullptr)
        return chunk;
    UpdateActivePage();
    return activePage->GetChunk();
}

uint PageCluster::NumOfPages() {
    uint size = availablePages.size() + unavailablePages.size();
    if (activePage != nullptr)
        size++;
    return size;
}

uint PageCluster::Capacity() {
    return (NumOfPages() * PAGE_CAPACITY(chunkSize));
}

uint PageCluster::NumOfObj() {
    uint numOfObj = 0;

    for (auto * p : availablePages)
        numOfObj += p->NumOfObj();

    for (auto * p : unavailablePages)
        numOfObj += p->NumOfObj();

    numOfObj += activePage->NumOfObj();
    return numOfObj;
}

void PageCluster::QueryPage() {
    std::byte * page = MemBank::GetPage();
    Page::Init(page, domain, chunkSize);
    availablePages.push_back((Page*)page);
    domain->totalNumOfPages++;
}

void PageCluster::UpdateActivePage() {
    // In this function we expect that
    // activePage has no free chunks (unavailable).
    if (availablePages.empty()) {
        if (domain->totalNumOfPages == domain->limitNumOfPages)
            return;
        QueryPage();
    }
    unavailablePages.push_back(activePage);
    activePage = availablePages.back();
    availablePages.pop_back();
}

void PageCluster::Mark() {
    for (auto * p : availablePages)
        p->Mark();

    for (auto * p : unavailablePages)
        p->Mark();

    activePage->Mark();
}

void PageCluster::Sweep() {
    for (auto * p : availablePages)
        p->Sweep();

    for (auto * p : unavailablePages)
        p->Sweep();

    activePage->Sweep();
}

void PageCluster::ReleaseEmptyPages_InVector(std::vector<Page*> & pages) {
    for (std::size_t i = 0; i < pages.size(); ) {
        if (pages[i]->IsEmpty()) {
            Page * page = pages[i];
            pages.erase(pages.begin() + i);
            domain->totalNumOfPages--;
            MemBank::AcceptPage((std::byte*)page);
        } else {
            i++;
        }
    }
}

void PageCluster::ReleaseEmptyPages() {
    ReleaseEmptyPages_InVector(availablePages);
    ReleaseEmptyPages_InVector(unavailablePages);
}

void PageCluster::AfterGc() {
    // Moving available pages from unavailable pages list
    // to available pages list.
    for (std::size_t i = 0; i < unavailablePages.size();) {
        if (unavailablePages[i]->HasFreeChunk()) {
            Page * page = unavailablePages[i];
            unavailablePages.erase(unavailablePages.begin() + i);
            availablePages.push_back(page);
        } else {
            i++;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////

MemDomain::MemDomain() {
    for (std::size_t i = 0; i < clusters.size(); i++) {
        clusters[i].domain = this;
        clusters[i].chunkSize = (i + 3) * ALIGNMENT;
        clusters[i].Init();
    }
    SetFlag_IsAvailable(true);
}

std::byte * MemDomain::GetChunk(uint chunkSize) {
    assert(chunkSize >= 24);
    assert(chunkSize <= 64);

    uint clusterIndex = 0;
    if (chunkSize % ALIGNMENT == 0) {
        clusterIndex = (chunkSize / ALIGNMENT) - 3;
    } else {
        // Actually clusterIndex = ((chunkSize / ALIGNMENT) + 1) - 3;
        clusterIndex = (chunkSize / ALIGNMENT) - 2;
    }

    auto & cluster = clusters[clusterIndex];
    return cluster.GetChunk();
}

uint MemDomain::NumOfPages() { return totalNumOfPages; }

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

void MemDomain::Gc() {
    lastMarked   = 0;
    lastDeleted  = 0;
    shrinkFactor = 0;

    for (auto & cluster : clusters)
        cluster.Mark();

    for (auto & cluster : clusters)
        cluster.Sweep();

    if (!GetFlag_IsBabyDomain()) {
        for (auto & cluster : clusters)
            cluster.ReleaseEmptyPages();
    }

    for (auto & cluster : clusters)
        cluster.AfterGc();

    // Computing shrink factor
    uint totalNumOfObj = lastMarked + lastDeleted;
    shrinkFactor = (double)lastDeleted / totalNumOfObj;

    SetFlag_IsAvailable(true);
}

void MemDomain::PrintStatus(const std::string & additionalMessage /* = "" */) {
    std::cout << '\n' << std::string(60, '-') << '\n';
    std::cout << "MemDomain status";

    if (!additionalMessage.empty())
        std::cout << " [" << additionalMessage << "]:" << "\n";
    else
        std::cout << "\n";

    uint width = 10;
    std::cout << "pages    : " << totalNumOfPages << '\n'
              << "capacity : " << Capacity() << '\n'
              << "objects  : " << NumOfObj() << '\n'
              << "memory   : " << Utils::NumSep(totalNumOfPages * 4) << " kb\n"
              << "shrink   : " << shrinkFactor << '\n';


    std::cout << "\npage clusters:\n";
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
        uint c_numOfPages = c.NumOfPages();
        uint fullCapacity = pageCapacity * c_numOfPages;
        uint numOfObjects = c.NumOfObj();
        uint occupancy    = ((double)numOfObjects / fullCapacity) * 100;

        std::cout << std::setw(width) << chunkSize
                  << std::setw(width) << pageCapacity
                  << std::setw(width) << c_numOfPages
                  << std::setw(width) << fullCapacity
                  << std::setw(width) << numOfObjects
                  << std::setw(width) << (std::to_string(occupancy) + " %")
                  << '\n';
    }
}

///////////////////////////////////////////////////////////////////////////////

MemDomain * Heap::constantDomain;
MemDomain * Heap::babyDomain;
uint        Heap::activeDomainIndex = 0;
MemDomain * Heap::activeDomain;
std::vector<MemDomain*> Heap::domains;

void (*Heap::PreDomainGc)(MemDomain * gcDomain);
void (*Heap::PreGlobalGc)();

void Heap::Init() {
    constantDomain = new MemDomain();
    constantDomain->SetFlag_IsConstant(true);
    constantDomain->limitNumOfPages = UINT32_MAX;

    babyDomain = new MemDomain();
    babyDomain->SetFlag_IsBabyDomain(true);

    domains.push_back(new MemDomain());
    activeDomain = domains[0];
}

std::byte * Heap::GetChunk_Constant(uint chunkSize) {
    return constantDomain->GetChunk(chunkSize);
}

std::byte * Heap::GetChunk_Baby(uint chunkSize) {
    std::byte * chunk = babyDomain->GetChunk(chunkSize);
    if (chunk != nullptr)
        return chunk;

    DomainGc(babyDomain);
    chunk = babyDomain->GetChunk(chunkSize);
    if (chunk != nullptr)
        return chunk;

    std::cerr << "Error. Can't allocate chunk in baby domain.";
    abort();
}

const double SHRINK_FACTOR_THRESHOLD = 0.2;

std::byte * Heap::GetChunk_Preferable(MemDomain * preferableDomain, uint chunkSize) {
    if (preferableDomain->GetFlag_IsAvailable()) {
        std::byte * chunk = preferableDomain->GetChunk(chunkSize);
        if (chunk != nullptr)
            return chunk;

        if (preferableDomain->shrinkFactor > SHRINK_FACTOR_THRESHOLD) {
            DomainGc(preferableDomain);
            chunk = preferableDomain->GetChunk(chunkSize);
            if (chunk != nullptr)
                return chunk;
        }
        preferableDomain->SetFlag_IsAvailable(false);
    }
    return GetChunk_Active(chunkSize);
}

std::byte * Heap::GetChunk_Active(uint chunkSize) {
    try_get_chunk:
    std::byte * chunk = activeDomain->GetChunk(chunkSize);
    if (chunk != nullptr)
        return chunk;

    if (activeDomain->shrinkFactor > SHRINK_FACTOR_THRESHOLD) {
        DomainGc(activeDomain);
        chunk = activeDomain->GetChunk(chunkSize);
        if (chunk != nullptr)
            return chunk;
    }
    activeDomain->SetFlag_IsAvailable(false);

    bool updateResult = UpdateActiveDomain();
    if (updateResult)
        goto try_get_chunk;

    GlobalGc();
    UpdateActiveDomain();
    chunk = activeDomain->GetChunk(chunkSize);
    if (chunk != nullptr)
        return chunk;
    std::cerr << "Error. Can't allocate chunk.";
    abort();
}

bool Heap::UpdateActiveDomain() {
    return false;
}

void Heap::DomainGc(MemDomain * domain) {
    PreDomainGc(domain);
    domain->Gc();
}

void Heap::GlobalGc() {
    PreGlobalGc();
}

void Heap::UpdateActiveDomain_AfterGlobalGc() {

}

///////////////////////////////////////////////////////////////////////////////

void Test_Mem() {
    Test_Page();
}
