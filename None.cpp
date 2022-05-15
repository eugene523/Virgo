#include "None.h"
#include "Type.h"

Type * None::t;

None * None::none;

void None::InitType() {
    None::t = new Type("none");
    none = (None*) Heap::GetChunk_Constant(sizeof(None));
    Obj::Init(none, None::t);
}
