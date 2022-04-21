#ifndef PROTON_OBJ_H
#define PROTON_OBJ_H

#include <string>
#include <memory>
#include <bitset>

using v_int  = long long int;
using v_real = long double;
using uint   = unsigned int;

struct Type;

struct Obj {
    const Type * const type;
    std::uint32_t numOfOwners;
    std::bitset<32> flags;
    explicit Obj(Type * type);
    inline bool Is(Type * ofType) { return type == ofType; }
};

#endif //PROTON_OBJ_H
