#ifndef PROTON_TYPE_H
#define PROTON_TYPE_H

#include <vector>
#include <map>

struct Obj;

struct MethodTable {
    void  (*Mark)      (Obj * self) {};
    void  (*Delete)    (Obj * self) {};
    Obj * (*Get)       (Obj * self, Obj * args) {};
    Obj * (*Set)       (Obj * self, Obj * args, Obj * val) {};
    Obj * (*GetField)  (Obj * self, Obj * fieldName) {};
    Obj * (*SetField)  (Obj * self, Obj * fieldName, Obj * val) {};
    Obj * (*ToStr)     (Obj * self) {};

    Obj * (*Eq)        (Obj * self, Obj * other) {};
    Obj * (*NotEq)     (Obj * self, Obj * other) {};

    Obj * (*Neg)       (Obj * self) {};
    Obj * (*Add)       (Obj * self, Obj * other) {};
    Obj * (*Sub)       (Obj * self, Obj * other) {};
    Obj * (*Mul)       (Obj * self, Obj * other) {};
    Obj * (*Div)       (Obj * self, Obj * other) {};
    Obj * (*Pow)       (Obj * self, Obj * other) {};
    Obj * (*Gr)        (Obj * self, Obj * other) {};
    Obj * (*GrEq)      (Obj * self, Obj * other) {};
    Obj * (*Ls)        (Obj * self, Obj * other) {};
    Obj * (*LsEq)      (Obj * self, Obj * other) {};

    Obj * (*Not)       (Obj * self) {};
    Obj * (*And)       (Obj * self, Obj * other) {};
    Obj * (*Or)        (Obj * self, Obj * other) {};

    // Used for debugging.
    std::string (*DebugStr) (Obj * self) {};
};

struct Type {
    const std::string name;
    MethodTable * methodTable;

    explicit Type(std::string name);
    ~Type();
};

#endif //PROTON_TYPE_H
