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

const unsigned int MemDom::MemDomSize = (1 << 8); // 256

MemDom::MemDom() {
    handlers.resize(MemDomSize);
    freeIndicesStack.resize(MemDomSize);
    for (int i = 0; i < MemDomSize; i++) {
        freeIndicesStack[i] = i;
    }
    freeIndicesStackTop = MemDomSize - 1;
}

Rf MemDom::NewRef(Obj * obj) {
    //
}

void MemDom::Mark() {
    for (int i = 0; i < MemDomSize; i++) {
        if (handlers[i].generation == generation)
            continue;

        if (handlers[i].numOfOwners > 0) {
            handlers[i].generation++;
            handlers[i].objPtr->Mark(this);
        }
    }
}

bool MemDom::HasFreeHandlers() {
    return freeIndicesStackTop >= 0;
}
