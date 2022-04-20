#include "None.h"
#include "Type.h"

Ref None_ToStr(Ref self) {}

Type * None::t;

void None::InitType() {
    None::t = new Type("none");
}

None::None() : Obj{None::t} {}
