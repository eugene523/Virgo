#ifndef VIRGO_OBJ_H
#define VIRGO_OBJ_H

#include <cassert>
#include <string>
#include <memory>
#include <bitset>
#include "Common.h"
#include "Type.h"
#include "Mem.h"

struct Obj {
    Type * type{};
    std::uint32_t numOfOwners{};

    enum
    {
        Bit_MarkColor
    };
    std::bitset<32> flags;

    static inline void Init(void * inPlace, Type * type) {
        assert(type != nullptr);
        Obj * obj = (Obj*)inPlace;
        obj->type = type;
        obj->numOfOwners = 0;
        obj->flags = 0;
        obj->flags[Bit_MarkColor] = Page::GetPage(inPlace)->domain->Get_MarkColor();
    }

    inline bool Is(Type * ofType) {
        return type == ofType;
    }

    inline void IncOwners() { numOfOwners++; }

    inline void DecOwners() { numOfOwners--; }

    inline bool Get_MarkColor() { return flags[Bit_MarkColor]; }

    inline void Mark() {
        flags[Bit_MarkColor].flip();
        assert(Page::GetPage(this)->domain->Get_MarkColor() == flags[Bit_MarkColor]);
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
