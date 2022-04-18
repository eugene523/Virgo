#ifndef PROTON_OBJ_H
#define PROTON_OBJ_H

#include <string>
#include <memory>

using v_int  = long long int;
using v_real = long double;
using uint   = unsigned int;

struct Type;

struct MemDomain;

struct Obj {
    const Type * const type;

    explicit Obj(Type * type);
    virtual ~Obj() = 0;
    bool Is(Type * ofType) const;

    [[nodiscard]]
    virtual std::string ToStr() const;

    virtual void Mark() {};

    //virtual void Mark(MemDomain * selfDom);
    //virtual void Delete(MemDomain * selfDom);
};

#endif //PROTON_OBJ_H
