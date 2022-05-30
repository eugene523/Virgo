#ifndef PROTON_SCRIPT_H
#define PROTON_SCRIPT_H

#include "Expr_.h"
#include "ContextStack.h"

class Script : public Obj, public IDefObj {
    Ref                 parentDefinition {};
    std::map<Ref, Ref>  childDefinitions;
    std::vector<Expr*>  expressions;

public:
    Ref self;
    static Type * t;
    static void InitType();
    Script();
    ~Script() override;
    void AddExpr(Expr * expr);
    void SetParentDef(Ref parent) override;
    Ref GetParentDef() override;
    void AddChildDef(Ref childName, Ref childRef) override;
    Ref GetChildDef(Ref childName) override;

    void PreExecution();
    static void PostExecution();
    Ref Execute();
    void Mark() override;
};

#endif //PROTON_SCRIPT_H
