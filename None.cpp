#include "None.h"
#include "Type.h"

Type * None::t;

void None::InitType() {
    None::t = new Type("none");
}

None::None() : Obj{None::t} {}

std::string None::ToStr() const {
    return "none";
}
