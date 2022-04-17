#include <cassert>
#include <iostream>
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
    freeHandlersStack.resize(size);
    for (int i = 0; i < size; i++) {
        handlers[i].domain = this;
        freeHandlersStack[i] = &handlers[i];
    }
    freeHandlersStackTop = size - 1;
}

Rf MemDom::NewRef(Obj * objPtr) {
    assert(freeHandlersStackTop >= 0);
    ObjHnd * objHndPtr = freeHandlersStack[freeHandlersStackTop];
    objHndPtr->objPtr = objPtr;
    objHndPtr->numOfOwners = 0;
    objHndPtr->generation = generation;
    Rf ref(objHndPtr);
    freeHandlersStackTop--;
    return ref;
}

void MemDom::FreeRef(Rf ref) {
    assert(ref.GetDomain() == this);
    ObjHnd * objHndPtr = ref.objHndPtr;
    freeHandlersStackTop++;
    freeHandlersStack[freeHandlersStackTop] = objHndPtr;

    objHndPtr->objPtr = nullptr;
    objHndPtr->numOfOwners = 0;
    objHndPtr->generation = 0;
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
            freeHandlersStackTop++;
            freeHandlersStack[freeHandlersStackTop] = &handlers[i];
            handlers[i].objPtr->Delete(this);

            // It's not necessary, but never the less it's better to clean handlers.
            handlers[i].objPtr = nullptr;
            handlers[i].numOfOwners = 0;
            handlers[i].generation = 0;
        }
    }
}

void MemDom::CollectGarbage() {
    Mark();
    Sweep();
}

bool MemDom::HasFreeHandlers() {
    return freeHandlersStackTop >= 0;
}

///////////////////////////////////////////////////////////////////////////////

void Heap::Init() {
    auto * memDom = new MemDom();
    domains.push_back(memDom);
    activeDomain = memDom;
}

Rf Heap::NewRef(Obj * objPtr) {
    if (activeDomain->HasFreeHandlers())
        return activeDomain->NewRef(objPtr);

    MarkTemp();
    PreCollectCallback();
    activeDomain->CollectGarbage();
    PostCollectCallback();
    UnMarkTemp();
    if (activeDomain->HasFreeHandlers())
        return activeDomain->NewRef(objPtr);

    auto * memDom = new MemDom();
    domains.push_back(memDom);
    activeDomain = memDom;
    return memDom->NewRef(objPtr);
}

void Heap::PushRefToTempStack(Rf ref) {
    if (refStackTop == RefStackCapacity) {
        std::cerr << "Error. Stack overflow." << std::endl;
        exit(1);
    }
    refStackTop++;
    refStack[refStackTop] = ref;
}

void Heap::PopRefFromTempStack() {
    assert(refStackTop >= 0);
    refStackTop--;
}

void Heap::MarkTemp() {
    for (int i = 0; i <= refStackTop; i++) {
        if (refStack[i].GetDomain() != activeDomain)
            continue;
        refStack[i].IncOwners();
    }
}

void Heap::UnMarkTemp() {
    for (int i = 0; i <= refStackTop; i++) {
        if (refStack[i].GetDomain() != activeDomain)
            continue;
        refStack[i].DecOwners();
    }
}

