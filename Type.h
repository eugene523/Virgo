#ifndef PROTON_TYPE_H
#define PROTON_TYPE_H

#include <vector>
#include <map>
#include "Mem.h"

struct OpTable {
    Ref  (*OpAdd)       (Ref a, Ref b) {};
    Ref  (*OpSub)       (Ref a, Ref b) {};
    Ref  (*OpMul)       (Ref a, Ref b) {};
    Ref  (*OpDiv)       (Ref a, Ref b) {};
    Ref  (*OpPow)       (Ref a, Ref b) {};
    Ref  (*OpNeg)       (Ref a) {};
    Ref  (*OpGr)        (Ref a, Ref b) {};
    Ref  (*OpGrEq)      (Ref a, Ref b) {};
    Ref  (*OpLs)        (Ref a, Ref b) {};
    Ref  (*OpLsEq)      (Ref a, Ref b) {};
    Ref  (*OpAnd)       (Ref a, Ref b) {};
    Ref  (*OpOr)        (Ref a, Ref b) {};
    Ref  (*OpNot)       (Ref a) {};
    Ref  (*OpEq)        (Ref a, Ref b) {};
    Ref  (*OpNotEq)     (Ref a, Ref b) {};
    Ref  (*Get)         (Ref self, Ref args) {};
    Ref  (*Set)         (Ref self, Ref args, Ref val) {};
    Ref  (*GetField)    (Ref self, Ref fieldName) {};
    Ref  (*SetField)    (Ref self, Ref fieldName, Ref val) {};
    Ref  (*ToStr)       (Ref self) {};
    void (*Mark)        (MemDomain * selfDomain) {};
    void (*Delete)      (MemDomain * selfDomain) {};
};

struct Type {
    const std::string name;
    OpTable * opTable;

    explicit Type(std::string name);
    ~Type();

    [[nodiscard]]
    std::string ToStr() const;
};

#endif //PROTON_TYPE_H
