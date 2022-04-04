#ifndef VIRGO_CLASS_H
#define VIRGO_CLASS_H

#include "Obj.h"
#include "IDefObj.h"

class Class : public Obj, public IDefObj {
    Ref self;
    Ref name;
    std::vector<Ref> fieldNames;
    Ref parentDefinition {};
    std::map<Ref, Ref> childDefinitions;

public:
    static Type * t;
    static void InitType();
    explicit Class(Ref name);
    void Mark() override;
    void SetParentDefinition(Ref parent) override;
    Ref  GetParentDefinition() override;
    void AddChildDefinition(Ref childName, Ref child) override;
    Ref  GetChildDefinition(Ref childName) override;
    void AddField(Ref fieldName);
    Ref CreateInstance(Ref args);
};

#endif //VIRGO_CLASS_H