#include "NFun.h"

#include <utility>
#include "Type.h"
#include "Str.h"

Type * NFun::t;

void NFun::InitType() {
    NFun::t = new Type("NFun");
}

NFun::NFun(std::string name) :
Obj{NFun::t}, name{std::move(name)} {}

NFun::~NFun() = default;

void NFun::Mark() {}

std::string NFun::ToStr() const {
    return name;
}


