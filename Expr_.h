#ifndef PROTON_EXPR_H
#define PROTON_EXPR_H

#include "Obj.h"
#include "Context_.h"

enum class ExprType {
    Undefined,
    Add,
    Sub,
    Mul,
    Div,
    Pow,
    Neg,
    Gr,
    GrEq,
    Ls,
    LsEq,
    And,
    Or,
    Not,
    Eq,
    NotEq,
    Literal,
    NameAccess,
    FieldAccess,
    CallAccess,
    ArgPair,
    Args,
    List,
    Assign,
    AddEq,
    SubEq,
    MulEq,
    DivEq,
    PowEq,
    Skip,
    Break,
    Return,
    If,
    For,
    ForC,
    ForIn,
    Call,
    Define,
};

struct Expr {
    const ExprType exprType;
    const uint line;
    Expr(ExprType exprType, uint line);
    virtual Ref Execute() = 0;
    virtual ~Expr() = default;
};

struct ExprUnary : Expr {
    Expr * a {};
    ExprUnary(ExprType exprType, Expr * a, uint line);
    ~ExprUnary() override;
};

struct ExprBinary : Expr {
    Expr * a {};
    Expr * b {};
    ExprBinary(ExprType exprType, Expr * a, Expr * b, uint line);
    ~ExprBinary() override;
};

struct ExprAccess : Expr {
    ExprAccess(ExprType exprType, uint line);
    virtual Ref Set(Ref value) = 0;
};

struct ExprAdd : ExprBinary {
    ExprAdd(Expr * a, Expr * b, uint line);
    Ref Execute() override;
};

struct ExprSub : ExprBinary {
    ExprSub(Expr * a, Expr * b, uint line);
    Ref Execute() override;
};

struct ExprMul : ExprBinary {
    ExprMul(Expr * a, Expr * b, uint line);
    Ref Execute() override;
};

struct ExprDiv : ExprBinary {
    ExprDiv(Expr * a, Expr * b, uint line);
    Ref Execute() override;
};

struct ExprPow : ExprBinary {
    ExprPow(Expr * a, Expr * b, uint line);
    Ref Execute() override;
};

struct ExprNeg : ExprUnary {
    ExprNeg(Expr * a, uint line);
    Ref Execute() override;
};

struct ExprGr : ExprBinary {
    ExprGr(Expr * a, Expr * b, uint line);
    Ref Execute() override;
};

struct ExprGrEq : ExprBinary {
    ExprGrEq(Expr * a, Expr * b, uint line);
    Ref Execute() override;
};

struct ExprLs : ExprBinary {
    ExprLs(Expr * a, Expr * b, uint line);
    Ref Execute() override;
};

struct ExprLsEq : ExprBinary {
    ExprLsEq(Expr * a, Expr * b, uint line);
    Ref Execute() override;
};

struct ExprAnd : ExprBinary {
    ExprAnd(Expr * a, Expr * b, uint line);
    Ref Execute() override;
};

struct ExprOr : ExprBinary {
    ExprOr(Expr * a, Expr * b, uint line);
    Ref Execute() override;
};

struct ExprNot : ExprUnary {
    ExprNot(Expr * a, uint line);
    Ref Execute() override;
};

struct ExprEq : ExprBinary {
    ExprEq(Expr * a, Expr * b, uint line);
    Ref Execute() override;
};

struct ExprNotEq : ExprBinary {
    ExprNotEq(Expr * a, Expr * b, uint line);
    Ref Execute() override;
};

struct ExprLiteral : Expr {
    Ref literal {};
    ExprLiteral(Ref literal, uint line);
    Ref Execute() override;
};

struct ExprNameAccess : ExprAccess {
    Ref name {};
    ExprNameAccess(Ref name, uint line);
    Ref Execute() override;
    Ref Set(Ref value) override;
};

struct ExprFieldAccess : ExprAccess {
    Expr * targetExpr {};
    Ref fieldName {};
    ExprFieldAccess(Expr * targetExpr, Ref fieldName, uint line);
    Ref Execute() override;
    Ref Set(Ref value) override;
};

struct ExprCallAccess : ExprAccess {
    Expr * targetExpr {};
    Expr * argsExpr {};
    ExprCallAccess(Expr * targetExpr, Expr * argsExpr, uint line);
    Ref Execute() override;
    Ref Set(Ref value) override;
};

struct ExprArgPair : Expr {
    Ref name {};
    Expr * evalValExpr {};
    ExprArgPair(Ref name, Expr * evalValExpr, uint line);
    Ref Execute() override;
};

struct ExprArgs : Expr {
    std::vector<Expr*> exprArgs;
    explicit ExprArgs(uint line);
    void AddExpr(Expr * expr);
    Ref Execute() override;
};

struct ExprList : Expr {
    std::vector<Expr*> exprList;
    explicit ExprList(uint line);
    void AddExpr(Expr * expr);
    uint NumOfElements();
    Ref Execute() override;
};

struct ExprAssign : Expr {
    ExprAccess * left;
    Expr * right;
    ExprAssign(ExprAccess * left, Expr * right, uint line);
    Ref Execute() override;
};

struct ExprAddEq : ExprBinary {
    ExprAddEq(Expr * a, Expr * b, uint line);
    Ref Execute() override;
};

struct ExprSubEq : ExprBinary {
    ExprSubEq(Expr * a, Expr * b, uint line);
    Ref Execute() override;
};

struct ExprMulEq : ExprBinary {
    ExprMulEq(Expr * a, Expr * b, uint line);
    Ref Execute() override;
};

struct ExprDivEq : ExprBinary {
    ExprDivEq(Expr * a, Expr * b, uint line);
    Ref Execute() override;
};

struct ExprPowEq : ExprBinary {
    ExprPowEq(Expr * a, Expr * b, uint line);
    Ref Execute() override;
};

struct ExprSkip : Expr {
    explicit ExprSkip(uint line);
    Ref Execute() override;
};

struct ExprBreak : Expr {
    explicit ExprBreak(uint line);
    Ref Execute() override;
};

struct ExprReturn : Expr {
    Expr * retExpr {};
    ExprReturn(Expr * retExpr, uint line);
    Ref Execute() override;
};

struct ExprIf : Expr {
    Expr * condition {};
    std::vector<Expr*> trueBranch;
    std::vector<Expr*> falseBranch;
    explicit ExprIf(uint line);
    Ref Execute() override;
};

struct ExprFor : Expr {
    Expr * condition {};
    std::vector<Expr*> expressions;
    explicit ExprFor(uint line);
    Ref Execute() override;
};

struct ExprForC : Expr {
    Expr * init {};
    Expr * condition {};
    Expr * post {};
    std::vector<Expr*> expressions;
    explicit ExprForC(uint line);
    Ref Execute() override;
};

struct ExprForIn : Expr {
    Ref name {};
    Expr * iterableExpr {};
    std::vector<Expr*> expressions;
    explicit ExprForIn(uint line);
    Ref Execute() override;
};

struct ExprDefine : Expr {
    Ref name {};
    Ref objRef {};
    ExprDefine(Ref name, Ref objRef, uint line);
    Ref Execute() override;
};

#endif //PROTON_EXPR_H
