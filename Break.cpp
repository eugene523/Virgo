#include "Break.h"
#include "Type.h"

Type * Break::t;

void Break::InitType() {
    Break::t = new Type("break");
}

Break::Break() : Obj{Break::t} {}

Break::~Break() = default;

void Break::Mark() {}
