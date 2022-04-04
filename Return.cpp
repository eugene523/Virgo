#include "Return.h"
#include "Type.h"

Type * Return::t;

void Return::InitType() {
    Return::t = new Type("return");
}

Return::Return(Ref retRef) : Obj{Return::t}, retRef{retRef} {}

void Return::Mark() {}
