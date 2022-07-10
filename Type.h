#ifndef VIRGO_TYPE_H
#define VIRGO_TYPE_H

#include <string>

struct Obj;

struct MethodTable {
    void  (*Mark)   (Obj * self) {};
    void  (*Delete) (Obj * self) {};
    Obj * (*Equal)  (Obj * self, Obj * other) {};
    Obj * (*ToStr)  (Obj * self) {};

    Obj * (*Negate)         (Obj * self) {};
    Obj * (*Add)            (Obj * self, Obj * other) {};
    Obj * (*Subtract)       (Obj * self, Obj * other) {};
    Obj * (*Multiply)       (Obj * self, Obj * other) {};
    Obj * (*Divide)         (Obj * self, Obj * other) {};
    Obj * (*Power)          (Obj * self, Obj * other) {};
    Obj * (*Greater)        (Obj * self, Obj * other) {};
    Obj * (*GreaterOrEqual) (Obj * self, Obj * other) {};
    Obj * (*Less)           (Obj * self, Obj * other) {};
    Obj * (*LessOrEqual)    (Obj * self, Obj * other) {};

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
