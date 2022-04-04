#include <sstream>
#include <iostream>
#include <utility>
#include "Type.h"

Type::Type(std::string name) : name{std::move(name)} {
    opTable = new OpTable();
}

std::string Type::ToStr() const {
    std::stringstream s;
    s  << name;
    return s.str();
}

Type::~Type() {
    delete opTable;
}