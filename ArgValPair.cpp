#include "ArgValPair.h"
#include "Type.h"

Type * ArgValPair::t;

void ArgValPair::InitType() {
    ArgValPair::t = new Type("ArgValPair");
}

ArgValPair::ArgValPair(Ref name, Ref val) :
Obj{ArgValPair::t}, name{name}, val{val} {}

ArgValPair::~ArgValPair() = default;

void ArgValPair::Mark() {
    GET_OBJ(name)->Mark();
    GET_OBJ(val)->Mark();
}
