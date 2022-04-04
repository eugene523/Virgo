#ifndef PROTON_SCRIPT_H
#define PROTON_SCRIPT_H

#include "Expr.h"
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
    void SetParentDefinition(Ref parent) override;
    Ref GetParentDefinition() override;
    void AddChildDefinition(Ref childName, Ref childRef) override;
    Ref GetChildDefinition(Ref childName) override;

    void PreExecution();
    static void PostExecution();
    Ref Execute();
    void Mark() override;
};

#endif //PROTON_SCRIPT_H
