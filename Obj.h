#ifndef PROTON_OBJ_H
#define PROTON_OBJ_H

#include <string>
#include <memory>

using v_int  = long long int;
using v_real = long double;
using uint   = unsigned int;

struct Type;

struct Obj {
    const Type * const type;
    explicit Obj(Type * type);
    inline bool Is(Type * ofType) { return type == ofType; }
};

#endif //PROTON_OBJ_H
