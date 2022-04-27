#include <sstream>
#include <utility>
#include "Type.h"
#include "Err.h"

Type * Err::t;

void Err::InitType() {
    t = new Type("err");
}

Err * Err::New(const std::string & message, uint srcLine /* = 0 */) {
    Err * err = (Err*)Heap::GetChunk(sizeof(Err));
    Obj::Init(err, Err::t);
    err->srcLine = srcLine;
    err->message = message;
    return err;
}

/*
std::string Err::ToStr() {
    std::stringstream s;
    if (srcLine > 0)
        s << "Line " << srcLine << ". ";
    s << "Error: " + message;
    return s.str();
}
*/