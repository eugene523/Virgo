#ifndef PROTON_IDEFOBJ_H
#define PROTON_IDEFOBJ_H

#include <string>
#include "Mem.h"

struct IDefObj {
    virtual void SetParentDefinition(Ref parent) = 0;
    virtual Ref  GetParentDefinition() = 0;
    virtual void AddChildDefinition(Ref childName, Ref child) = 0;
    virtual Ref  GetChildDefinition(Ref childName) = 0;
};

#endif //PROTON_IDEFOBJ_H
