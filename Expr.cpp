#include <cassert>
#include <sstream>
#include "Expr.h"
#include "Type.h"
#include "Err.h"
#include "Bool.h"
#include "Str.h"
#include "List.h"
#include "Iter.h"
#include "ArgPair.h"
#include "Args.h"
#include "Skip.h"
#include "Break.h"
#include "Return.h"
#include "Fun.h"
#include "NFun.h"
#include "Class.h"
#include "VM.h"

const char * ERR_EXPR_ARG_IS_NULL = "Null reference passed as an argument of the expression.";

///////////////////////////////////////////////////////////////////////////////////////////////////

#define EXPR_UNARY_IMPL(OpFun, OpName) \
Ref a_ref = a->Execute(); \
Obj * a_obj = GET_OBJ(a_ref); \
\
if (a_obj == nullptr) \
    return NEW_REF(new Err(ERR_EXPR_ARG_IS_NULL, line)); \
\
if (a_obj->Is(Err::t)) { \
    auto err = (Err*)a_obj; \
    if (err->line == 0) \
        err->line = line; \
    return a_ref; \
} \
PUSH_TEMP(a_ref); \
\
auto op = a_obj->type->opTable->OpFun; \
if (op == nullptr) { \
    POP_TEMP; \
    return Err_NoSuchOperation(a_obj, OpName, line); \
} \
\
Ref result = op(a_ref); \
POP_TEMP; \
return result; \

///////////////////////////////////////////////////////////////////////////////////////////////////

#define EXPR_BINARY_IMPL(OpFun, OpName) \
Ref a_ref = a->Execute(); \
Obj * a_obj = GET_OBJ(a_ref); \
if (a_obj->Is(Err::t)) { \
    auto err = (Err*)a_obj; \
    if (err->line == 0) \
        err->line = line; \
    return a_ref; \
} \
PUSH_TEMP(a_ref); \
\
Ref b_ref = b->Execute(); \
Obj * b_obj = GET_OBJ(b_ref); \
if (b_obj->Is(Err::t)) { \
    POP_TEMP; \
    auto err = (Err*)b_obj; \
    if (err->line == 0) \
        err->line = line; \
    return b_ref; \
} \
PUSH_TEMP(b_ref); \
\
auto op = a_obj->type->opTable->OpFun; \
if (op == nullptr) { \
    POP_TEMP; \
    POP_TEMP; \
    return Err_NoSuchOperation(a_obj, OpName, line); \
} \
\
Ref result = op(a_ref, b_ref); \
POP_TEMP; \
POP_TEMP; \
return result; \

///////////////////////////////////////////////////////////////////////////////////////////////////

#define EXPR_BINARY_EQ_IMPL(OpFun, OpName) \
Ref a_ref = a->Execute(); \
Obj * a_obj = GET_OBJ(a_ref); \
if (a_obj->Is(Err::t)) { \
    auto err = (Err*)a_obj; \
    if (err->line == 0) \
        err->line = line; \
    return a_ref; \
} \
PUSH_TEMP(a_ref); \
\
Ref b_ref = b->Execute(); \
Obj * b_obj = GET_OBJ(b_ref); \
if (b_obj->Is(Err::t)) { \
    POP_TEMP; \
    auto err = (Err*)b_obj; \
    if (err->line == 0) \
        err->line = line; \
    return b_ref; \
} \
PUSH_TEMP(b_ref); \
\
auto op = a_obj->type->opTable->OpFun; \
if (op == nullptr) { \
    POP_TEMP; \
    POP_TEMP; \
    return Err_NoSuchOperation(a_obj, OpName, line); \
} \
\
Ref resultRef = op(a_ref, b_ref); \
Obj * resultObj = GET_OBJ(resultRef); \
if (resultObj->Is(Err::t)) { \
    POP_TEMP; \
    POP_TEMP; \
    auto err = (Err*)resultObj; \
    if (err->line == 0) \
        err->line = line; \
    return resultRef; \
} \
PUSH_TEMP(resultRef); \
\
Ref assignResultRef = ((ExprAccess*)a)->Set(resultRef); \
Obj * assignResultObj = GET_OBJ(assignResultRef); \
if (assignResultObj->Is(Err::t)) { \
    auto err = (Err*)assignResultObj; \
    if (err->line == 0) \
        err->line = line; \
} \
POP_TEMP; \
POP_TEMP; \
POP_TEMP; \
return assignResultRef; \

///////////////////////////////////////////////////////////////////////////////////////////////////

Ref Err_NoSuchOperation(Obj * obj, const std::string & opName, uint line) {
    std::stringstream s;
    s << "Object of type '" << obj->type->name 
      << "' does not provide '"<< opName << "' operation.";
    return NEW_REF(new Err(s.str(), line));
}

///////////////////////////////////////////////////////////////////////////////////////////////////

Expr::Expr(ExprType exprType, uint line) : exprType{exprType}, line{line} {}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprUnary::ExprUnary(ExprType exprType, Expr * a, uint line) :
Expr{exprType, line}, a{a} {}

ExprUnary::~ExprUnary() {
    delete a;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprBinary::ExprBinary(ExprType exprType, Expr * a, Expr * b, uint line) :
Expr{exprType, line}, a{a}, b{b} {}

ExprBinary::~ExprBinary() {
    delete a;
    delete b;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprAccess::ExprAccess(ExprType exprType, uint line) : Expr{exprType, line} {}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprAdd::ExprAdd(Expr * a, Expr * b, uint line) :
ExprBinary{ExprType::Add, a, b, line} {}

Ref ExprAdd::Execute() {
    EXPR_BINARY_IMPL(OpAdd, "+")
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprSub::ExprSub(Expr * a, Expr * b, uint line) :
ExprBinary{ExprType::Sub, a, b, line} {}

Ref ExprSub::Execute() {
    EXPR_BINARY_IMPL(OpSub, "-")
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprMul::ExprMul(Expr * a, Expr * b, uint line) :
ExprBinary{ExprType::Mul, a, b, line} {}

Ref ExprMul::Execute() {
    EXPR_BINARY_IMPL(OpMul, "*")
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprDiv::ExprDiv(Expr * a, Expr * b, uint line) :
ExprBinary{ExprType::Div, a, b, line} {}

Ref ExprDiv::Execute() {
    EXPR_BINARY_IMPL(OpDiv, "/")
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprPow::ExprPow(Expr * a, Expr * b, uint line) :
ExprBinary{ExprType::Pow, a, b, line} {}

Ref ExprPow::Execute() {
    EXPR_BINARY_IMPL(OpPow, "^")
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprNeg::ExprNeg(Expr * a, uint line) :
ExprUnary{ExprType::Neg, a, line} {}

Ref ExprNeg::Execute() {
    EXPR_UNARY_IMPL(OpNeg, "-")
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprGr::ExprGr(Expr * a, Expr * b, uint line) :
ExprBinary{ExprType::Gr, a, b, line} {}

Ref ExprGr::Execute() {
    EXPR_BINARY_IMPL(OpGr, ">")
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprGrEq::ExprGrEq(Expr * a, Expr * b, uint line) :
ExprBinary{ExprType::GrEq, a, b, line} {}

Ref ExprGrEq::Execute() {
    EXPR_BINARY_IMPL(OpGrEq, ">=")
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprLs::ExprLs(Expr * a, Expr * b, uint line) :
ExprBinary{ExprType::Ls, a, b, line} {}

Ref ExprLs::Execute() {
    EXPR_BINARY_IMPL(OpLs, "<")
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprLsEq::ExprLsEq(Expr * a, Expr * b, uint line) :
ExprBinary{ExprType::LsEq, a, b, line} {}

Ref ExprLsEq::Execute() {
    EXPR_BINARY_IMPL(OpLsEq, "<=")
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprAnd::ExprAnd(Expr * a, Expr * b, uint line) :
ExprBinary{ExprType::And, a, b, line} {}

Ref ExprAnd::Execute() {
    EXPR_BINARY_IMPL(OpAnd, "and")
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprOr::ExprOr(Expr * a, Expr * b, uint line) :
ExprBinary{ExprType::Or, a, b, line} {}

Ref ExprOr::Execute() {
    EXPR_BINARY_IMPL(OpOr, "or")
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprNot::ExprNot(Expr * a, uint line) :
ExprUnary{ExprType::Not, a, line} {}

Ref ExprNot::Execute() {
    EXPR_UNARY_IMPL(OpNot, "not")
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprEq::ExprEq(Expr * a, Expr * b, uint line) :
ExprBinary{ExprType::Eq, a, b, line} {}

Ref ExprEq::Execute() {
    EXPR_BINARY_IMPL(OpEq, "=")
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprNotEq::ExprNotEq(Expr * a, Expr * b, uint line) :
ExprBinary{ExprType::NotEq, a, b, line} {}

Ref ExprNotEq::Execute() {
    EXPR_BINARY_IMPL(OpNotEq, "!=")
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprLiteral::ExprLiteral(Ref literal, uint line) :
Expr{ExprType::Literal, line}, literal{literal} {}

Ref ExprLiteral::Execute() {
    return literal;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprNameAccess::ExprNameAccess(Ref name, uint line) :
ExprAccess{ExprType::NameAccess, line}, name{name} {}

Ref ExprNameAccess::Execute() {
    return VM::contextStack.Last()->GetVariable(name);
}

Ref ExprNameAccess::Set(Ref value) {
    VM::contextStack.Last()->SetVariable(name, value);
    return Ref::none;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprFieldAccess::ExprFieldAccess(Expr * targetExpr, Ref fieldName, uint line) :
ExprAccess{ExprType::FieldAccess, line}, targetExpr{targetExpr}, fieldName{fieldName} {}

Ref ExprFieldAccess::Execute() {
    Ref targetRef = targetExpr->Execute();
    Obj * targetObj = GET_OBJ(targetRef);

    if (targetObj == nullptr)
        return NEW_REF(new Err("Can't perform field access operation on null object.", line));

    if (targetObj->Is(Err::t)) {
        auto err = (Err*)targetObj;
        if (err->line == 0)
            err->line = line;
        return targetRef;
    }
    PUSH_TEMP(targetRef);

    Ref result;
    auto op = targetObj->type->opTable->GetField;
    if (op == nullptr) {
        POP_TEMP;
        return Err_NoSuchOperation(targetObj, "GetField", line);
    }

    result = op(targetRef, fieldName);
    POP_TEMP;
    return result;
}

Ref ExprFieldAccess::Set(Ref value) {
    Ref targetRef = targetExpr->Execute();
    Obj * targetObj = GET_OBJ(targetRef);

    if (targetObj == nullptr)
        return NEW_REF(new Err("Can't perform field access operation on null object.", line));

    if (targetObj->Is(Err::t)) {
        auto err = (Err*)targetObj;
        if (err->line == 0)
            err->line = line;
        return targetRef;
    }
    PUSH_TEMP(targetRef);

    Ref result;
    auto op = targetObj->type->opTable->SetField;
    if (op == nullptr) {
        POP_TEMP;
        return Err_NoSuchOperation(targetObj, "GetField", line);
    }

    result = op(targetRef, fieldName, value);
    POP_TEMP;
    return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprCallAccess::ExprCallAccess(Expr * targetExpr, Expr * argsExpr, uint line) :
ExprAccess{ExprType::CallAccess, line}, targetExpr{targetExpr}, argsExpr{argsExpr} {}

Ref ExprCallAccess::Execute() {
    Ref targetRef = targetExpr->Execute();
    Obj * targetObj = GET_OBJ(targetRef);

    if (targetObj == nullptr)
        return NEW_REF(new Err("Can't perform call operation on null object.", line));

    if (targetObj->Is(Err::t)) {
        auto err = (Err*)targetObj;
        if (err->line == 0)
            err->line = line;
        return targetRef;
    }
    PUSH_TEMP(targetRef);

    Ref argsRef = argsExpr->Execute();
    Obj * argsObj = GET_OBJ(argsRef);
    if (argsObj == nullptr) {
        POP_TEMP;
        return NEW_REF(new Err("argsObj is null.", line));
    }

    if (argsObj->Is(Err::t)) {
        POP_TEMP;
        auto err = (Err*)argsObj;
        if (err->line == 0)
            err->line = line;
        return argsRef;
    }
    PUSH_TEMP(argsRef);

    Ref result;
    if (targetObj->Is(Fun::t)) {
        auto fun = (Fun*)targetObj;
        result = fun->Execute(argsRef);
        POP_TEMP;
        POP_TEMP;
        return result;
    }
    else if (targetObj->Is(NFun::t)) {
        auto fun = (NFun*)targetObj;
        result = fun->Execute(argsRef);
        POP_TEMP;
        POP_TEMP;
        return result;
    }
    else if (targetObj->Is(Class::t)) {
        auto cl = (Class*)targetObj;
        result = cl->CreateInstance(argsRef);
        POP_TEMP;
        POP_TEMP;
        return result;
    }
    else {
        POP_TEMP;
        POP_TEMP;
        return NEW_REF(new Err("Object is not callable"));
    }

    auto op = targetObj->type->opTable->Get;
    if (op == nullptr) {
        POP_TEMP;
        POP_TEMP;
        return Err_NoSuchOperation(targetObj, "Get", line);
    }

    result = op(targetRef, argsRef);
    POP_TEMP;
    POP_TEMP;
    return result;
}

Ref ExprCallAccess::Set(Ref value) {
    Ref targetRef = targetExpr->Execute();
    Obj * targetObj = GET_OBJ(targetRef);

    if (targetObj == nullptr)
        return NEW_REF(new Err("Can't perform assignment operation on null object.", line));

    if (targetObj->Is(Err::t)) {
        auto err = (Err*)targetObj;
        if (err->line == 0)
            err->line = line;
        return targetRef;
    }
    PUSH_TEMP(targetRef);

    Ref argsRef = argsExpr->Execute();
    Obj * argsObj = GET_OBJ(argsRef);
    if (argsObj == nullptr) {
        POP_TEMP;
        return NEW_REF(new Err("argsObj is null.", line));
    }

    if (argsObj->Is(Err::t)) {
        POP_TEMP;
        auto err = (Err*)argsObj;
        if (err->line == 0)
            err->line = line;
        return argsRef;
    }
    PUSH_TEMP(argsRef);

    auto op = targetObj->type->opTable->Set;
    if (op == nullptr) {
        POP_TEMP;
        POP_TEMP;
        return Err_NoSuchOperation(targetObj, "Set", line);
    }

    Ref result = op(targetRef, argsRef, value);
    POP_TEMP;
    POP_TEMP;
    return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprArgPair::ExprArgPair(Ref name, Expr * evalValPair, uint line) :
Expr{ExprType::ArgPair, line}, name{name}, evalValExpr{evalValPair} {}

Ref ExprArgPair::Execute() {
    Ref val = evalValExpr->Execute();
    return NEW_REF(new ArgPair(name, val));
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprArgs::ExprArgs(uint line) :
Expr{ExprType::Args, line} {}

Ref ExprArgs::Execute() {
    uint numOfItems = 0;
    Ref result;
    auto * argList = new Args();
    for (auto i : exprArgs) {
        Ref argRef = i->Execute();
        Obj * argObj = GET_OBJ(argRef);
        if (argObj->Is(Err::t)) {
            auto err = (Err*)argObj;
            if (err->line == 0)
                err->line = line;
            result = argRef;
            goto return_result;
        }
        PUSH_TEMP(argRef);
        argList->Add(argRef);
        numOfItems++;
    }

    result = NEW_REF(argList);
    return_result:
    for (uint i = 0; i < numOfItems; i++) {
        POP_TEMP;
    }
    return result;
}

void ExprArgs::AddExpr(Expr * expr) {
    exprArgs.push_back(expr);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprList::ExprList(uint line) : Expr{ExprType::List, line} {}

Ref ExprList::Execute() {
    uint numOfItems = 0;
    Ref result;
    auto * list = new List();
    for (auto i : exprList) {
        Ref ref = i->Execute();
        Obj * obj = GET_OBJ(ref);
        if (obj->Is(Err::t)) {
            auto err = (Err*)obj;
            if (err->line == 0)
                err->line = line;
            result = ref;
            goto return_result;
        }
        PUSH_TEMP(ref);
        list->elements.push_back(ref);
        numOfItems++;
    }

    result = NEW_REF(list);
    return_result:
    for (uint i = 0; i < numOfItems; i++) {
        POP_TEMP;
    }
    return result;
}

void ExprList::AddExpr(Expr * expr) {
    exprList.push_back(expr);
}

uint ExprList::NumOfElements() {
    return exprList.size();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprAssign::ExprAssign(ExprAccess * left, Expr * right, uint line) :
Expr{ExprType::Assign, line}, left{left}, right{right} {}

Ref ExprAssign::Execute() {
    Ref rightRef = right->Execute();
    Obj * rightObj = GET_OBJ(rightRef);
    if (rightObj->Is(Err::t)) {
        auto err = (Err*)rightObj;
        if (err->line == 0)
            err->line = line;
        return rightRef;
    }
    PUSH_TEMP(rightRef);

    Ref assignResultRef = left->Set(rightRef);
    Obj * assignResultObj = GET_OBJ(assignResultRef);
    if (assignResultObj->Is(Err::t)) {
        auto err = (Err*)assignResultObj;
        if (err->line == 0)
            err->line = line;
    }
    POP_TEMP;
    return assignResultRef;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprAddEq::ExprAddEq(Expr * a, Expr * b, uint line) :
ExprBinary{ExprType::AddEq, a, b, line} {}

Ref ExprAddEq::Execute() {
    EXPR_BINARY_EQ_IMPL(OpAdd, "+")
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprSubEq::ExprSubEq(Expr * a, Expr * b, uint line) :
ExprBinary{ExprType::SubEq, a, b, line} {}

Ref ExprSubEq::Execute() {
    EXPR_BINARY_EQ_IMPL(OpSub, "-")
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprMulEq::ExprMulEq(Expr * a, Expr * b, uint line) :
ExprBinary{ExprType::MulEq, a, b, line} {}

Ref ExprMulEq::Execute() {
    EXPR_BINARY_EQ_IMPL(OpMul, "*")
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprDivEq::ExprDivEq(Expr * a, Expr * b, uint line) :
ExprBinary{ExprType::DivEq, a, b, line} {}

Ref ExprDivEq::Execute() {
    EXPR_BINARY_EQ_IMPL(OpDiv, "/")
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprPowEq::ExprPowEq(Expr * a, Expr * b, uint line) :
ExprBinary{ExprType::PowEq, a, b, line} {}

Ref ExprPowEq::Execute() {
    EXPR_BINARY_EQ_IMPL(OpPow, "^")
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprSkip::ExprSkip(uint line) : Expr{ExprType::Skip, line} {}

Ref ExprSkip::Execute() {
    return NEW_REF(new Skip());
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprBreak::ExprBreak(uint line) : Expr{ExprType::Break, line} {}

Ref ExprBreak::Execute() {
    return NEW_REF(new Break());
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprReturn::ExprReturn(Expr * retExpr, uint line) :
Expr{ExprType::Return, line}, retExpr{retExpr} {}

Ref ExprReturn::Execute() {
    if (retExpr == nullptr)
        return NEW_REF(new Return(Ref::none));
    Ref retValRef = retExpr->Execute();
    PUSH_TEMP(retValRef);
    Ref retRef = NEW_REF(new Return(retValRef));
    POP_TEMP;
    return retRef;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprIf::ExprIf(uint line) : Expr{ExprType::If, line} {}

Ref ExprIf::Execute() {
    Ref result = condition->Execute();
    Obj * resultObj = GET_OBJ(result);
    
    if (resultObj->Is(Err::t))
        return result;

    if (!resultObj->Is(Bool::t))
        return NEW_REF(new Err("Condition result must be of boolean type.", line));

    bool r = ((Bool*)resultObj)->val;
    if (r) {
        for (auto i : trueBranch) {
            result = i->Execute();
            resultObj = GET_OBJ(result);
            if (result != Ref::none) {
                if (i->exprType == ExprType::Return ||
                    resultObj->Is(Skip::t) ||
                    resultObj->Is(Break::t) ||
                    resultObj->Is(Err::t))
                    return result;
            }
        }
    } else {
        for (auto i : falseBranch) {
            result = i->Execute();
            resultObj = GET_OBJ(result);
            if (result != Ref::none) {
                if (i->exprType == ExprType::Return ||
                    resultObj->Is(Skip::t) ||
                    resultObj->Is(Break::t) ||
                    resultObj->Is(Err::t))
                    return result;
            }
        }
    }
    return Ref::none;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprFor::ExprFor(uint line) : Expr{ExprType::For, line} {}

Ref ExprFor::Execute() {
    for(;;) {
        Ref result = condition->Execute();
        Obj * resultObj = GET_OBJ(result);

        if (resultObj->Is(Err::t))
            return result;

        if (!resultObj->Is(Bool::t))
            return NEW_REF(new Err("Condition result must be of boolean type.", line));

        bool r = ((Bool*)resultObj)->val;
        if (r) {
            for (auto i : expressions) {
                result = i->Execute();
                resultObj = GET_OBJ(result);
                if (result != Ref::none) {
                    if (i->exprType == ExprType::Return || resultObj->Is(Err::t))
                        return result;
                    
                    if (resultObj->Is(Skip::t))
                        break;
                    
                    if (resultObj->Is(Break::t))
                        return Ref::none;
                }
            }
        }
        else return Ref::none;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprForC::ExprForC(uint line) : Expr{ExprType::ForC, line} {}

Ref ExprForC::Execute() {
    Ref initResult = init->Execute();
    Obj * initResultObj = GET_OBJ(initResult);

    if (initResultObj->Is(Err::t))
        return initResult;

    for(;;) {
        Ref result = condition->Execute();
        Obj * resultObj = GET_OBJ(result);
        
        if (resultObj->Is(Err::t))
            return result;
        
        if (!resultObj->Is(Bool::t))
            return NEW_REF(new Err("Condition result must be of boolean type.", line));
        
        auto r = ((Bool*)resultObj)->val;
        if (r) {
            for (auto i : expressions) {
                result = i->Execute();
                resultObj = GET_OBJ(result);
                if (result != Ref::none) {
                    if (i->exprType == ExprType::Return || resultObj->Is(Err::t))
                        return result;
                    
                    if (resultObj->Is(Skip::t))
                        break;
                    
                    if (resultObj->Is(Break::t))
                        return Ref::none;
                }
            }
            result = post->Execute();
            resultObj = GET_OBJ(result);
            if (resultObj->Is(Err::t))
                return result;
        }
        else return Ref::none;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprForIn::ExprForIn(uint line) : Expr{ExprType::ForIn, line} {}

Ref ExprForIn::Execute() {
    Ref iterableRef = iterableExpr->Execute();
    Obj * iterableObj = GET_OBJ(iterableRef);
    if (iterableObj->Is(Err::t))
        return iterableRef;
    PUSH_TEMP(iterableRef);

    Ref result;
    auto * iter = new Iter(iterableRef);
    if (!iter->IsAvailable()) {
        result = NEW_REF(new Err("Object is not iterable."));
        goto return_result;
    }

    while (iter->HasNext()) {
        Ref next = iter->Next();
        VM::contextStack.Last()->SetVariable(name, next);
        for (auto i : expressions) {
            result = i->Execute();
            auto * resultObj = GET_OBJ(result);
            if (result != Ref::none) {
                if (i->exprType == ExprType::Return || resultObj->Is(Err::t))
                    goto return_result;

                if (resultObj->Is(Skip::t))
                    break;

                if (resultObj->Is(Break::t)) {
                    result = Ref::none;
                    goto return_result;
                }
            }
        }
    }
    return_result:
    POP_TEMP;
    return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprDefine::ExprDefine(Ref name, Ref objRef, uint line) :
Expr{ExprType::Define, line}, name{name}, objRef{objRef} {}

Ref ExprDefine::Execute() { return Ref::none; }
