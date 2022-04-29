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
    Type *        type{};
    std::uint32_t numOfOwners{};

    enum
    {
        Obj_MarkColorBit
    };
    std::bitset<32> flags;

    static inline void Init(void * inPlace, Type * type) {
        assert(type != nullptr);
        Obj * obj = (Obj*)inPlace;
        obj->type = type;
        obj->numOfOwners = 0;
        obj->flags = 0;
        obj->flags[Obj_MarkColorBit] = GET_PAGE(inPlace)->domain->Get_MarkColor();
    }

    inline bool Is(Type * ofType) { return type == ofType; }

    inline bool Get_MarkColor() { return flags[Obj_MarkColorBit]; }

    inline void Mark() { flags[Obj_MarkColorBit] = !flags[Obj_MarkColorBit]; }
};

#define IS_OF_TYPE(obj, ofType) (((Obj*)obj)->type == ofType)

#endif //VIRGO_OBJ_H
