#include "ArgPair.h"
#include "Type.h"

Type * ArgPair::t;

void ArgPair::InitType() {
    ArgPair::t = new Type("ArgPair");
}

ArgPair::ArgPair(Ref name, Ref val) :
Obj{ArgPair::t}, name{name}, val{val} {}

ArgPair::~ArgPair() = default;

void ArgPair::Mark() {
    Mem::MarkRef(val);
    GET_OBJ(val)->Mark();
}
