#ifndef VIRGO_OBJ_H
#define VIRGO_OBJ_H

#include <cassert>
#include <string>
#include <memory>
#include <bitset>

using v_int  = long long int;
using v_real = long double;
using uint   = unsigned int;

struct Type;

struct Obj {
    Type * type{};
    std::uint32_t numOfOwners{};
    std::bitset<32> flags;

    static inline void Init(void * inPlace, Type * type) {
        assert(type != nullptr);
        Obj * obj = (Obj*)inPlace;
        obj->type = type;
        obj->numOfOwners = 0;
        obj->flags = 0;
    }

    inline bool Is(Type * ofType) { return type == ofType; }
};

#define IS_OF_TYPE(obj, ofType) (((Obj*)obj)->type == ofType)

#endif //VIRGO_OBJ_H
