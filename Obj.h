#ifndef VIRGO_OBJ_H
#define VIRGO_OBJ_H

#include <cassert>
#include <string>
#include <memory>
#include <bitset>

struct Type;

enum ObjFlags
{
    IsMarked,
    IsConstant,
};

// We do inherit all types from this object.
struct Obj {
    std::bitset<64> flags{};
    Type * type{};

    explicit Obj(Type * type_);

    Type * GetType();
    bool Is(Type * ofType);

    bool GetFlag_IsMarked();
    void SetFlag_IsMarked(bool value);

    bool GetFlag_IsConstant();
    void SetFlag_IsConstant(bool value);

    void Delete();
};

#endif //VIRGO_OBJ_H
