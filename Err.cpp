#include <sstream>
#include "Type.h"
#include "Err.h"

Type * Err::t;

void Err::InitType() {
    Err::t = new Type("err");
}

Err::Err(std::string message, uint line /* = 0 */) :
Obj{Err::t}, message{std::move(message)}, line{line} {}

/*
std::string Err::ToStr() const {
    std::stringstream s;
    if (line > 0)
        s << "Line " << line << ". ";
    s << "Error: " + message;
    return s.str();
}
*/