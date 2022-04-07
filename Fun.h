#ifndef PROTON_FUNCTION_H
#define PROTON_FUNCTION_H

#include "Obj.h"
#include "IDefObj.h"
#include "Expr.h"

class Fun : public Obj, public IDefObj {
    Ref                 self;
    Ref                 name;
    uint                numOfArgs {};
    uint                numOfRequiredArgs {};
    std::vector<Ref>    argNames;
    std::vector<Ref>    argDefaultValues;
    std::map<Ref, uint> argPositions;
    Ref                 parentDef {};
    std::map<Ref, Ref>  childDefs;
    std::vector<Expr*>  expressions;

    void PrepareContext();
    Ref ValidateArguments(Ref argList);
    Ref PrepareArguments(Ref argList);
    Ref PerformExecution();
    static void PostExecution();

    Ref Err_NotEnoughArgs(uint numOfPassedArgs);
    Ref Err_TooManyArgs(uint numOfPassedArgs);
    Ref Err_NoSuchArg(Ref passedArgName);
    Ref Err_ArgIsNotSet(Ref argName);

public:
    static Type * t;
    static void InitType();
    explicit Fun(Ref name);
    ~Fun() override;
    Ref  GetName();
    void Mark() override;
    void SetSelf(Ref selfRef);
    void AddArgument(Ref argName, Ref argDefaultValue = Ref::none);
    void AddExpression(Expr * expr);
    void SetParentDef(Ref parent) override;
    Ref  GetParentDef() override;
    void AddChildDef(Ref childName, Ref child) override;
    Ref  GetChildDef(Ref childName) override;
    Ref  Execute(Ref argList);
};

#endif //PROTON_FUNCTION_H
