#ifndef VIRGO_TYPE_H
#define VIRGO_TYPE_H

#include <string>

struct Obj;

struct MethodTable {
    void  (*Mark)   (Obj * self) {};
    void  (*Delete) (Obj * self) {};
    Obj * (*Equal)  (Obj * self, Obj * other) {};
    Obj * (*ToStr)  (Obj * self) {};

    Obj * (*Neg)    (Obj * self) {};
    Obj * (*Add)    (Obj * self, Obj * other) {};
    Obj * (*Sub)    (Obj * self, Obj * other) {};
    Obj * (*Mul)    (Obj * self, Obj * other) {};
    Obj * (*Div)    (Obj * self, Obj * other) {};
    Obj * (*Pow)    (Obj * self, Obj * other) {};
    Obj * (*Gr)     (Obj * self, Obj * other) {};
    Obj * (*GrEq)   (Obj * self, Obj * other) {};
    Obj * (*Ls)     (Obj * self, Obj * other) {};
    Obj * (*LsEq)   (Obj * self, Obj * other) {};

    // Used for debugging.
    std::string (*DebugStr) (Obj * self) {};
};

struct Type {
    const std::string name;
    MethodTable * methodTable;

    explicit Type(std::string name);
    ~Type();
};

#endif //VIRGO_TYPE_H
