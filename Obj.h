#ifndef VIRGO_OBJ_H
#define VIRGO_OBJ_H

#include <cassert>
#include <string>
#include <memory>
#include <bitset>
#include "Common.h"
#include "Type.h"
#include "Mem.h"

// This is actually a header for any object.
struct Obj {
    std::uint32_t numOfOwners{};

    enum
    {
        Flag_IsMarked
    };
    std::bitset<32> flags;

    Type * type{};

    static inline void Init(void * inPlace, Type * type) {
        assert(type != nullptr);
        Obj * obj = (Obj*)inPlace;
        obj->type = type;
        obj->numOfOwners = 0;
        obj->flags = 0;
    }

    inline bool Is(Type * ofType) { return type == ofType; }

    inline void IncOwners() { numOfOwners++; }
    inline void DecOwners() { numOfOwners--; }

    inline bool GetFlag_IsMarked() { return flags[Flag_IsMarked]; }
    inline void ResetFlag_IsMarked() { flags[Flag_IsMarked] = false; }

    inline void Mark() {
        flags[Flag_IsMarked] = true;
        auto markMethod = type->methodTable->Mark;
        if (markMethod == nullptr)
            return;
        markMethod(this);
    }

    inline void Delete() {
        auto deleteMethod = type->methodTable->Delete;
        if (deleteMethod == nullptr)
            return;
        deleteMethod(this);
    }
};

#endif //VIRGO_OBJ_H
