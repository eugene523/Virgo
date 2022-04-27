#ifndef PROTON_TYPE_H
#define PROTON_TYPE_H

#include <vector>
#include <map>
#include "Mem.h"

struct MethodTable {
    void  (*Mark)        (Obj * self) {};
    void  (*Delete)      (Obj * self) {};
    Obj * (*Get)         (Obj * self, Obj * args) {};
    Obj * (*Set)         (Obj * self, Obj * args, Obj * val) {};
    Obj * (*GetField)    (Obj * self, Obj * fieldName) {};
    Obj * (*SetField)    (Obj * self, Obj * fieldName, Obj * val) {};
    Obj * (*ToStr)       (Obj * self) {};

    Obj * (*OpAdd)       (Obj * self, Obj * other) {};
    Obj * (*OpSub)       (Obj * self, Obj * other) {};
    Obj * (*OpMul)       (Obj * self, Obj * other) {};
    Obj * (*OpDiv)       (Obj * self, Obj * other) {};
    Obj * (*OpPow)       (Obj * self, Obj * other) {};
    Obj * (*OpNeg)       (Obj * self) {};
    Obj * (*OpGr)        (Obj * self, Obj * other) {};
    Obj * (*OpGrEq)      (Obj * self, Obj * other) {};
    Obj * (*OpLs)        (Obj * self, Obj * other) {};
    Obj * (*OpLsEq)      (Obj * self, Obj * other) {};
    Obj * (*OpAnd)       (Obj * self, Obj * other) {};
    Obj * (*OpOr)        (Obj * self, Obj * other) {};
    Obj * (*OpNot)       (Obj * self) {};
    Obj * (*OpEq)        (Obj * self, Obj * other) {};
    Obj * (*OpNotEq)     (Obj * self, Obj * other) {};
};

struct Type {
    const std::string name;
    MethodTable * methodTable;

    explicit Type(std::string name);
    ~Type();
};

#endif //PROTON_TYPE_H
