#include "Skip.h"
#include "Type.h"

Type * Skip::t;

void Skip::InitType() {
    Skip::t = new Type("skip");
}

Skip::Skip() : Obj{Skip::t} {}

void Skip::Mark() {}
