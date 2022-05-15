#include <sstream>
#include <utility>
#include "Type.h"
#include "Error.h"

Type * Error::t;

void Error::InitType() {
    t = new Type("error");
}

Error * Error::New(const std::string & message, uint srcLine /* = 0 */) {
    Error * err = (Error*) Heap::GetChunk_Baby(sizeof(Error));
    Obj::Init(err, Error::t);
    err->srcLine = srcLine;
    err->message = message;
    return err;
}

/*
std::string Error::ToStr() {
    std::stringstream s;
    if (srcLine > 0)
        s << "Line " << srcLine << ". ";
    s << "Error: " + message;
    return s.str();
}
*/