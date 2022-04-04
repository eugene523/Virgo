#ifndef PROTON_MEM_H
#define PROTON_MEM_H

#include <vector>
#include <map>
#include <cstdint>
#include "Obj.h"

struct ObjHandler {
    Obj * objPtr {};
    std::size_t generation {};
    bool preserve {false};

    ObjHandler() = default;
    ObjHandler(Obj * objPtr, std::size_t gcGen);
};

///////////////////////////////////////////////////////////////////////////////

struct Ref {
    std::size_t index {};

    Ref() = default;
    explicit Ref(std::size_t index);
    bool operator==(const Ref & another) const;
    bool operator!=(const Ref & another) const;
    bool operator<(const Ref & another) const;
    static const Ref none;
};

///////////////////////////////////////////////////////////////////////////////

struct Mem {
    static std::vector<ObjHandler> handlers;
    static std::vector<std::size_t> freeIndicesStack;
    static long long int freeIndicesStackTop;
    static std::size_t currentGeneration;
    static std::size_t numOfCreatedObjects;

    // Temporary references stack is used during evaluating expressions.
    // We push to this stack intermediate results of a currently evaluating expression,
    // so we can be sure that intermediate values will not be deleted if GC will start
    // in the middle of expression.
    static const std::size_t maxTempRefSize;
    static std::vector<Ref> tempRefStack;
    static long long int tempRefStackTop;

    static void (*MarkCallback) ();

    static void Init();
    [[nodiscard]] static Ref NewRef(Obj * obj);
    [[nodiscard]] static Ref NewPreservedRef(Obj * obj);
    static void MakeRefPreserved(Ref ref);
    static Obj * GetObj(Ref ref);
    static void MarkRef(Ref ref);
    static void MarkTemp();
    static void Sweep();
    static void Extend();
    static void PushRefToTempStack(Ref ref);
    static void PopRefFromTempStack();
    static void PrintStatus();
    static void PrintMem();
};

#define GET_OBJ(ref) Mem::GetObj(ref)

#define NEW_REF(obj) Mem::NewRef(obj)

#define NEW_PRESERVED_REF(obj) Mem::NewPreservedRef(obj)

#define PUSH_TEMP(ref) Mem::PushRefToTempStack(ref)

#define POP_TEMP Mem::PopRefFromTempStack()

#endif //PROTON_MEM_H
