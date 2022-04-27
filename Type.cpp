#include <iostream>
#include <utility>
#include "Type.h"

Type::Type(std::string name) : name{std::move(name)} {
    methodTable = new MethodTable();
}

Type::~Type() {
    delete methodTable;
}