#ifndef VIRGO_TYPE_H
#define VIRGO_TYPE_H

#include <string>
#include "Obj.h"

struct MethodTable {
    void  (*Mark)   (Obj * self) {};
    void  (*Delete) (Obj * self) {};
    Obj * (*Equal)  (Obj * self, Obj * other) {};
    Obj * (*ToStr)  (Obj * self, std::byte * inPlace) {};

    Obj * (*Neg)  (Obj * self, std::byte * inPlace) {};
    Obj * (*Add)  (Obj * self, Obj * other, std::byte * inPlace) {};
    Obj * (*Sub)  (Obj * self, Obj * other, std::byte * inPlace) {};
    Obj * (*Mul)  (Obj * self, Obj * other, std::byte * inPlace) {};
    Obj * (*Div)  (Obj * self, Obj * other, std::byte * inPlace) {};
    Obj * (*Pow)  (Obj * self, Obj * other, std::byte * inPlace) {};

    Obj * (*Gr)   (Obj * self, Obj * other) {};
    Obj * (*GrEq) (Obj * self, Obj * other) {};
    Obj * (*Ls)   (Obj * self, Obj * other) {};
    Obj * (*LsEq) (Obj * self, Obj * other) {};

    // Used for debugging.
    std::string (*DebugStr) (Obj * self) {};
};

struct Type : Obj {
    const std::string name;
    MethodTable * methodTable;

    explicit Type(std::string name);
    ~Type();
};

#endif //VIRGO_TYPE_H
