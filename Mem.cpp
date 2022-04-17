#include <cassert>
#include <iostream>
#include <iomanip>
#include "Mem.h"
#include "VM.h"

ObjHandler::ObjHandler(Obj * objPtr, std::size_t gcGen) :
objPtr{objPtr}, generation{gcGen} {}

///////////////////////////////////////////////////////////////////////////////

Ref::Ref(std::size_t index) : index{index} {}

bool Ref::operator==(const Ref & another) const {
    return index == another.index;
}

bool Ref::operator!=(const Ref & another) const {
    return index != another.index;
}

bool Ref::operator<(const Ref & another) const {
    return index < another.index;
}

const Ref Ref::none{};

///////////////////////////////////////////////////////////////////////////////

std::vector<ObjHandler>  Mem::handlers;
std::vector<std::size_t> Mem::freeIndicesStack;
long long int            Mem::freeIndicesStackTop;
std::size_t              Mem::currentGeneration{1};
std::size_t              Mem::numOfCreatedObjects;
const std::size_t        Mem::maxTempRefSize{1024};
std::vector<Ref>         Mem::tempRefStack;
long long int            Mem::tempRefStackTop{-1};

void (*Mem::MarkCallback) ();

void Mem::Init() {
    // Initial number of handlers
    const std::size_t n = (1 << 16);
    handlers.resize(n);
    freeIndicesStack.resize(n);
    for (std::size_t i = 0; i < n; i++) {
        freeIndicesStack[i] = (n - 1) - i;
    }
    freeIndicesStackTop = n - 1;
    tempRefStack.resize(maxTempRefSize);
}

Ref Mem::NewRef(Obj * obj) {
    if (freeIndicesStackTop == -1) {
        MarkCallback();
        MarkTemp();
        Sweep();
        if (freeIndicesStackTop == -1) {
            Extend();
            if (freeIndicesStackTop == -1) {
                std::cerr << "Error. Not enough memory." << std::endl;
                exit(1);
            }
        }
    }
    std::size_t index = freeIndicesStack[freeIndicesStackTop];
    freeIndicesStack[freeIndicesStackTop] = 0; // Less UB.
    freeIndicesStackTop--;
    ObjHandler h(obj, currentGeneration);
    handlers[index] = h;
    numOfCreatedObjects++;
    return Ref(index);
}

Ref Mem::NewPreservedRef(Obj* obj) {
    Ref r = NewRef(obj);
    MakeRefPreserved(r);
    return r;
}

void Mem::MakeRefPreserved(Ref ref) {
    handlers[ref.index].preserve = true;
}

Obj * Mem::GetObj(Ref ref) {
    return handlers[ref.index].objPtr;
}

void Mem::MarkRef(Ref ref) {
    if (handlers[ref.index].generation == (currentGeneration + 1)) // It's already marked.
        return;
    handlers[ref.index].generation++;
    handlers[ref.index].objPtr->Mark();
}

void Mem::MarkTemp() {
    for (long long int i = 0; i <= tempRefStackTop; i++)
        MarkRef(tempRefStack[i]);
}

void Mem::Sweep() {
    for (std::size_t i = 0; i < handlers.size(); i++) {
        if (handlers[i].preserve)
            continue;

        if (handlers[i].generation > currentGeneration)
            continue;

        delete handlers[i].objPtr;
        handlers[i].objPtr = nullptr;
        handlers[i].generation = 0; // Less UB
        freeIndicesStackTop++;
        freeIndicesStack[freeIndicesStackTop] = i;
        numOfCreatedObjects--;
    }
    currentGeneration++;
}

void Mem::Extend() {
    std::size_t oldCap = handlers.capacity();
    std::size_t addCap = oldCap; // Capacity will be twice as before.
    std::size_t newCap = oldCap + addCap;
    handlers.resize(newCap);
    freeIndicesStack.resize(newCap);
    for (std::size_t i = 0; i < addCap; i++) {
        freeIndicesStack[freeIndicesStackTop + 1 + i] = (newCap - 1) - i;
    }
    freeIndicesStackTop += addCap;
}

void Mem::PushRefToTempStack(Ref ref) {
    if (tempRefStackTop == maxTempRefSize) {
        std::cerr << "Error. Stack overflow." << std::endl;
        exit(1);
    }
    tempRefStackTop++;
    tempRefStack[tempRefStackTop] = ref;
}

void Mem::PopRefFromTempStack() {
    assert(tempRefStackTop >= 0);
    tempRefStackTop--;
}

void Mem::PrintStatus() {
    std::cout << "\nMemory status:";
    std::cout << "\nnumOfCreatedObjects = " << numOfCreatedObjects << std::endl;
    std::cout << "currentGeneration = " << currentGeneration << std::endl;
    std::cout << "numOfHandlers = " << handlers.size() << std::endl;
    std::cout << "freeHandlersStackTop = " << freeIndicesStackTop << std::endl;
    std::cout << "tempRefStackTop = " << tempRefStackTop << std::endl;
}

#include "Type.h"

void Mem::PrintMem() {
    const int w = 10;
    std::cout << "\nMemory content:";
    for (std::size_t i = 0; i < handlers.size(); i++) {
        std::cout << "\n";
        std::cout << std::left << std::setw(w) << i;
        std::string isPreserved = handlers[i].preserve ? "pres" : "-";
        std::cout << std::left << std::setw(w) << isPreserved;
        std::cout << std::left << std::setw(w) << handlers[i].generation;
        std::cout << std::left << std::setw(w) << handlers[i].objPtr;
        if (handlers[i].objPtr != nullptr) {
            Type * t = (Type*)handlers[i].objPtr->type;
            std::cout << std::left << std::setw(w) << t->ToStr();
            std::cout << std::left << std::setw(w) << handlers[i].objPtr->ToStr();
        }
    }
}
