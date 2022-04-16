#include <cassert>
#include "Mem2.h"

///////////////////////////////////////////////////////////////////////////////

const Rf Rf::none{};

Rf::Rf(ObjHnd * objHndPtr) : objHndPtr{objHndPtr} {}

bool Rf::operator==(const Rf & another) const {
    return objHndPtr == another.objHndPtr;
}

bool Rf::operator!=(const Rf & another) const {
    return objHndPtr != another.objHndPtr;
}

bool Rf::operator<(const Rf & another) const {
    return objHndPtr < another.objHndPtr;
}

///////////////////////////////////////////////////////////////////////////////

const unsigned int MemDom::size = (1 << 8); // 256

MemDom::MemDom() {
    handlers.resize(size);
    idxStack.resize(size);
    for (int i = 0; i < size; i++) {
        idxStack[i] = i;
        handlers[i].domain = this;
    }
    idxStackTop = size - 1;
}

Rf MemDom::NewRef(Obj * objPtr) {
    assert(idxStackTop >= 0);
    int i = idxStack[idxStackTop];
    handlers[i].objPtr = objPtr;
    handlers[i].generation = generation;
    Rf ref(&handlers[i]);
    idxStackTop--;
    return ref;
}

void MemDom::Mark() {
    for (int i = 0; i < size; i++) {
        if (handlers[i].generation == generation)
            continue;

        if (handlers[i].numOfOwners > 0) {
            handlers[i].generation++;
            handlers[i].objPtr->Mark(this);
        }
    }
}

void MemDom::Sweep() {
    for (int i = 0; i < size; i++) {
        if (handlers[i].generation < generation) {
            assert(handlers[i].numOfOwners == 0);
            idxStackTop++;
            idxStack[idxStackTop] = i;
            handlers[i].objPtr->Delete(this);

            // It's not necessary, but never the less it's better to clean handlers.
            handlers[i].objPtr = nullptr;
            handlers[i].generation = 0;
        }
    }
}

void MemDom::PullUp() { }

void MemDom::CollectGarbage() {
    Mark();
    Sweep();
    //PullUp();
}

bool MemDom::HasFreeHandlers() {
    return idxStackTop >= 0;
}

///////////////////////////////////////////////////////////////////////////////

void Heap::MarkTemp() {
    for (int i = 0; i <= refStackTop; i++) {
        if (refStack[i].GetDomain() != activeDomain)
            continue;
        refStack[i].IncOwners();
    }
}

Rf Heap::NewRef(Obj * objPtr) {
    if (activeDomain->HasFreeHandlers())
        return activeDomain->NewRef(objPtr);

    MarkTemp();
    PreCollectCallback();
    activeDomain->CollectGarbage();
    PostCollectCallback();
    if (activeDomain->HasFreeHandlers())
        return activeDomain->NewRef(objPtr);

    auto * memDom = new MemDom();
    domains.push_back(memDom);
    activeDomain = memDom;
    return memDom->NewRef(objPtr);
}


