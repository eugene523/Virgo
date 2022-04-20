#include <cassert>
#include <iostream>
#include <sstream>
#include "Mem.h"
#include "Type.h"

///////////////////////////////////////////////////////////////////////////////

const Ref Ref::none{};

Ref::Ref(ObjHnd * objHndPtr) : objHnd{objHndPtr} {}

bool Ref::operator==(const Ref & another) const {
    return objHnd == another.objHnd;
}

bool Ref::operator!=(const Ref & another) const {
    return objHnd != another.objHnd;
}

bool Ref::operator<(const Ref & another) const {
    return objHnd < another.objHnd;
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

Ref MemDomain::NewRef(Obj * obj) {
    assert(freeHandlersStackTop >= 0);
    ObjHnd * objHnd = freeHandlersStack[freeHandlersStackTop];
    objHnd->obj = obj;
    objHnd->numOfOwners = 0;
    objHnd->generation = generation;
    Ref ref(objHnd);
    freeHandlersStack[freeHandlersStackTop] = nullptr;
    freeHandlersStackTop--;
    return ref;
}

void MemDomain::FreeRef(Ref ref) {
    assert(ref.GetDomain() == this);
    ObjHnd * objHnd = ref.objHnd;
    freeHandlersStackTop++;
    freeHandlersStack[freeHandlersStackTop] = objHnd;

    objHnd->obj = nullptr;
    objHnd->numOfOwners = 0;
    objHnd->generation = 0;
}

void MemDomain::Mark() {
    for (int i = 0; i < size; i++) {
        if (handlers[i].generation == generation)
            continue;

        if (handlers[i].numOfOwners > 0) {
            handlers[i].generation++;
            auto * markFun = handlers[i].obj->type->opTable->Mark;
            if (markFun != nullptr)
                markFun(this);
        }
    }
}

void MemDomain::Sweep() {
    for (int i = 0; i < size; i++) {
        if (handlers[i].generation < generation) {
            assert(handlers[i].numOfOwners == 0);
            freeHandlersStackTop++;
            freeHandlersStack[freeHandlersStackTop] = &handlers[i];
            auto * deleteFun = handlers[i].obj->type->opTable->Delete;
            if (deleteFun != nullptr)
                deleteFun(this);

            // It's not necessary, but never the less it's better to clean handlers.
            handlers[i].obj = nullptr;
            handlers[i].numOfOwners = 0;
            handlers[i].generation = 0;
        }
    }
}

void MemDomain::CollectGarbage() {
    assert(!isConstant);
    generation++;
    Mark();
    Sweep();
}

bool MemDomain::HasFreeHandlers() {
    return freeHandlersStackTop >= 0;
}

void MemDomain::PrintStatus(const std::string & additionalMessage /* = "" */) {
    std::stringstream s;

    if (!additionalMessage.empty())
        s << "\n-- " << additionalMessage << "--";

    s << "\nMemDomain info:\n"
      << "isConstant           : " << std::boolalpha << isConstant << '\n'
      << "size                 : " << size << '\n'
      << "generation           : " << generation << '\n'
      << "hasFreeHandlers      : " << HasFreeHandlers() << '\n'
      << "numOfObjects         : " << NumOfObjects() << '\n'
      << "freeHandlersStackTop : " << freeHandlersStackTop << '\n';

    std::cout << s.str();
}

///////////////////////////////////////////////////////////////////////////////

std::vector<MemDomain*> Heap::domains{};
MemDomain * Heap::constantDomain{};
MemDomain * Heap::babyDomain{};
MemDomain * Heap::activeDomain{};
const int Heap::RefStackCapacity{1 << 10}; // 1024
std::vector<Ref> Heap::refStack{};
int Heap::refStackTop{};

void (*Heap::PreCollectCallback) () {};
void (*Heap::PostCollectCallback) () {};

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
    refStack[refStackTop].objHnd = nullptr;
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
