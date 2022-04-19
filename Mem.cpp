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
    freeHandlersStack[freeHandlersStackTop] = nullptr;
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
            handlers[i].objPtr->type->opTable->Mark(this);
        }
    }
}

void MemDomain::Sweep() {
    for (int i = 0; i < size; i++) {
        if (handlers[i].generation < generation) {
            assert(handlers[i].numOfOwners == 0);
            freeHandlersStackTop++;
            freeHandlersStack[freeHandlersStackTop] = &handlers[i];
            handlers[i].objPtr->type->opTable->Delete(this);

            // It's not necessary, but never the less it's better to clean handlers.
            handlers[i].objPtr = nullptr;
            handlers[i].numOfOwners = 0;
            handlers[i].generation = 0;
        }
    }
}

void MemDomain::CollectGarbage() {
    assert(!isConstant);
    Mark();
    Sweep();
}

bool MemDomain::HasFreeHandlers() {
    return freeHandlersStackTop >= 0;
}

///////////////////////////////////////////////////////////////////////////////

void Heap::Init() {
    constantDomain = new MemDomain();
    constantDomain->isConstant = true;

    babyDomain = new MemDomain();

    domains.push_back(new MemDomain());
    activeDomain = domains[0];
}

void Heap::CollectGarbageInDomain(MemDomain * targetDomain) {
    MarkTemp();
    PreCollectCallback();
    targetDomain->CollectGarbage();
    PostCollectCallback();
    UnmarkTemp();
}

Ref Heap::NewRef(Obj * objPtr) {
    if (activeDomain->HasFreeHandlers())
        return activeDomain->NewRef(objPtr);

    CollectGarbageInDomain(activeDomain);

    if (activeDomain->HasFreeHandlers())
        return activeDomain->NewRef(objPtr);

    auto * memDom = new MemDomain();
    domains.push_back(memDom);
    activeDomain = memDom;
    return memDom->NewRef(objPtr);
}

Ref Heap::NewRefInDomain(Obj * objPtr, MemDomain * targetDomain) {
    if (targetDomain->HasFreeHandlers())
        return targetDomain->NewRef(objPtr);

    CollectGarbageInDomain(targetDomain);

    if (targetDomain->HasFreeHandlers())
        return targetDomain->NewRef(objPtr);

    return NewRef(objPtr);
}

Ref Heap::NewRefNeighbour(Obj * objPtr, Ref neighbour) {
    return NewRefInDomain(objPtr, neighbour.GetDomain());
}

Ref Heap::NewPreservedRef(Obj * objPtr) {
    Ref ref = NewRef(objPtr);
    ref.IncOwners();
    return ref;
}

Ref Heap::NewConstantRef(Obj* objPtr) {
    assert(constantDomain->HasFreeHandlers());
    return constantDomain->NewRef(objPtr);
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
    refStack[refStackTop].objHndPtr = nullptr;
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
