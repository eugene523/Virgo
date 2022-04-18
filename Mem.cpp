#include <cassert>
#include <iostream>
#include "Mem.h"

///////////////////////////////////////////////////////////////////////////////

const Ref Ref::none{};

Ref::Ref(ObjHnd * objHndPtr) : objHndPtr{objHndPtr} {}

bool Ref::operator==(const Ref & another) const {
    return objHndPtr == another.objHndPtr;
}

bool Ref::operator!=(const Ref & another) const {
    return objHndPtr != another.objHndPtr;
}

bool Ref::operator<(const Ref & another) const {
    return objHndPtr < another.objHndPtr;
}

///////////////////////////////////////////////////////////////////////////////

const unsigned int MemDomain::size = (1 << 8); // 256

MemDomain::MemDomain() {
    handlers.resize(size);
    freeHandlersStack.resize(size);
    for (int i = 0; i < size; i++) {
        handlers[i].domain = this;
        freeHandlersStack[i] = &handlers[i];
    }
    freeHandlersStackTop = size - 1;
}

Ref MemDomain::NewRef(Obj * objPtr) {
    assert(freeHandlersStackTop >= 0);
    ObjHnd * objHndPtr = freeHandlersStack[freeHandlersStackTop];
    objHndPtr->objPtr = objPtr;
    objHndPtr->numOfOwners = 0;
    objHndPtr->generation = generation;
    Ref ref(objHndPtr);
    freeHandlersStackTop--;
    return ref;
}

void MemDomain::FreeRef(Ref ref) {
    assert(ref.GetDomain() == this);
    ObjHnd * objHndPtr = ref.objHndPtr;
    freeHandlersStackTop++;
    freeHandlersStack[freeHandlersStackTop] = objHndPtr;

    objHndPtr->objPtr = nullptr;
    objHndPtr->numOfOwners = 0;
    objHndPtr->generation = 0;
}

#include "Type.h"

void MemDomain::Mark() {
    for (int i = 0; i < size; i++) {
        if (handlers[i].generation == generation)
            continue;

        if (handlers[i].numOfOwners > 0) {
            handlers[i].generation++;
            handlers[i].objPtr->Mark(this);
            handlers[i].objPtr->type->opTable->Mark(handlers[i]);
        }
    }
}

void MemDomain::Sweep() {
    for (int i = 0; i < size; i++) {
        if (handlers[i].generation < generation) {
            assert(handlers[i].numOfOwners == 0);
            freeHandlersStackTop++;
            freeHandlersStack[freeHandlersStackTop] = &handlers[i];
            handlers[i].objPtr->->Delete(this);
            handlers[i]

            // It's not necessary, but never the less it's better to clean handlers.
            handlers[i].objPtr = nullptr;
            handlers[i].numOfOwners = 0;
            handlers[i].generation = 0;
        }
    }
}

void MemDomain::CollectGarbage() {
    Mark();
    Sweep();
}

bool MemDomain::HasFreeHandlers() {
    return freeHandlersStackTop >= 0;
}

///////////////////////////////////////////////////////////////////////////////

void Heap::Init() {
    activeDomain = new MemDomain();
    activeDomain->isActiveDomain = true;

    constantDomain = new MemDomain();

    auto * memDom = new MemDomain();
    domains.push_back(memDom);
    activeDomain = memDom;
}

Ref Heap::NewRef(Obj * objPtr) {
    if (activeDomain->HasFreeHandlers())
        return activeDomain->NewRef(objPtr);

    MarkTemp();
    PreCollectCallback();
    activeDomain->CollectGarbage();
    PostCollectCallback();
    UnmarkTemp();
    if (activeDomain->HasFreeHandlers())
        return activeDomain->NewRef(objPtr);

    auto * memDom = new MemDomain();
    domains.push_back(memDom);
    activeDomain = memDom;
    return memDom->NewRef(objPtr);
}

Ref Heap::NewPreservedRef(Obj * objPtr) {
    Ref ref = NewRef(objPtr);
    ref.IncOwners();
    return ref;
}

void Heap::PushRef(Ref ref) {
    if (refStackTop == RefStackCapacity) {
        std::cerr << "Error. Stack overflow." << std::endl;
        exit(1);
    }
    refStackTop++;
    refStack[refStackTop] = ref;
}

void Heap::PopRef() {
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

void Heap::UnmarkTemp() {
    for (int i = 0; i <= refStackTop; i++) {
        if (refStack[i].GetDomain() != activeDomain)
            continue;
        refStack[i].DecOwners();
    }
}

