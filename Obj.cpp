#include "Obj.h"
#include "Type.h"

Obj::Obj(Type * type) : type{type} {}

Obj::~Obj() = default;

bool Obj::Is(Type * ofType) const {
    return type == ofType;
}

std::string Obj::ToStr() const {
    return type->name;
}
