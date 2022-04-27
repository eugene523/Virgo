#include "None.h"
#include "Type.h"

Type * None::t;

None * None::constant;

void None::InitType() {
    None::t = new Type("none");
    constant = (None*)Heap::GetChunk_Const(sizeof(None));
    Obj::Init(constant, None::t);
}
