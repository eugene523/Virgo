#include "Obj.h"
#include "Type.h"

Obj::Obj(Type * type): type{type} {}

bool Obj::Is(Type * ofType) {
    return type == ofType;
}

bool Obj::GetFlag_IsMarked() {
    return flags[ObjFlags::IsMarked];
}

void Obj::SetFlag_IsMarked(bool value) {
    flags[ObjFlags::IsMarked] = value;
}

bool Obj::GetFlag_IsConstant() {
    return flags[ObjFlags::IsConstant];
}

void Obj::SetFlag_IsConstant(bool value) {
    flags[ObjFlags::IsConstant] = value;
}

void Obj::Delete() {
    auto deleteMethod = type->methodTable->Delete;
    if (deleteMethod == nullptr)
        return;
    deleteMethod(this);
}
