#ifndef PROTON_IDEFOBJ_H
#define PROTON_IDEFOBJ_H

#include <string>
#include "Mem.h"

struct IDefObj {
    virtual void SetParentDef(Ref parent) = 0;
    virtual Ref  GetParentDef() = 0;
    virtual void AddChildDef(Ref childName, Ref child) = 0;
    virtual Ref  GetChildDef(Ref childName) = 0;
};

#endif //PROTON_IDEFOBJ_H
