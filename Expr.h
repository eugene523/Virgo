#ifndef VIRGO_EXPR_H
#define VIRGO_EXPR_H

#include <vector>
#include "Common.h"
#include "ByteCode.h"

enum class ExprType
{
    Undefined,
    LoadConstant,
    GetLocalVariable,
    SetLocalVariable,
    Eq,
    NotEq,
    Neg,
    Add,
    Sub,
    Mul,
    Div,
    Pow,
    Gr,
    GrEq,
    Ls,
    LsEq,
    Not,
    And,
    Or,
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
    virtual void Compile(ByteCode & bc) = 0;
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

struct ExprLoadConstant : Expr {
    uint id {};
    ExprLoadConstant(uint id, uint line);
    void Compile(ByteCode & bc) override;
};

struct ExprGetLocalVariable : Expr {
    uint id {};
    explicit ExprGetLocalVariable(uint id, uint line);
    void Compile(ByteCode & bc) override;
};

struct ExprSetLocalVariable : Expr {
    uint id {};
    Expr * valueExpr {};
    explicit ExprSetLocalVariable(uint id, Expr * valueExpr, uint line);
    void Compile(ByteCode & bc) override;
};

struct ExprEq : ExprBinary {
    ExprEq(Expr * a, Expr * b, uint line);
    void Compile(ByteCode & bc) override;
};

struct ExprNotEq : ExprBinary {
    ExprNotEq(Expr * a, Expr * b, uint line);
    void Compile(ByteCode & bc) override;
};

struct ExprNeg : ExprUnary {
    ExprNeg(Expr * a, uint line);
    void Compile(ByteCode & bc) override;
};

struct ExprAdd : ExprBinary {
    ExprAdd(Expr * a, Expr * b, uint line);
    void Compile(ByteCode & bc) override;
};

struct ExprSub : ExprBinary {
    ExprSub(Expr * a, Expr * b, uint line);
    void Compile(ByteCode & bc) override;
};

struct ExprMul : ExprBinary {
    ExprMul(Expr * a, Expr * b, uint line);
    void Compile(ByteCode & bc) override;
};

struct ExprDiv : ExprBinary {
    ExprDiv(Expr * a, Expr * b, uint line);
    void Compile(ByteCode & bc) override;
};

struct ExprPow : ExprBinary {
    ExprPow(Expr * a, Expr * b, uint line);
    void Compile(ByteCode & bc) override;
};

struct ExprGr : ExprBinary {
    ExprGr(Expr * a, Expr * b, uint line);
    void Compile(ByteCode & bc) override;
};

struct ExprGrEq : ExprBinary {
    ExprGrEq(Expr * a, Expr * b, uint line);
    void Compile(ByteCode & bc) override;
};

struct ExprLs : ExprBinary {
    ExprLs(Expr * a, Expr * b, uint line);
    void Compile(ByteCode & bc) override;
};

struct ExprLsEq : ExprBinary {
    ExprLsEq(Expr * a, Expr * b, uint line);
    void Compile(ByteCode & bc) override;
};

struct ExprNot : ExprUnary {
    ExprNot(Expr * a, uint line);
    void Compile(ByteCode & bc) override;
};

struct ExprAnd : ExprBinary {
    ExprAnd(Expr * a, Expr * b, uint line);
    void Compile(ByteCode & bc) override;
};

struct ExprOr : ExprBinary {
    ExprOr(Expr * a, Expr * b, uint line);
    void Compile(ByteCode & bc) override;
};

struct ExprAddEq : ExprBinary {
    ExprAddEq(Expr * a, Expr * b, uint line);
    void Compile(ByteCode & bc) override;
};

struct ExprSubEq : ExprBinary {
    ExprSubEq(Expr * a, Expr * b, uint line);
    void Compile(ByteCode & bc) override;
};

struct ExprMulEq : ExprBinary {
    ExprMulEq(Expr * a, Expr * b, uint line);
    void Compile(ByteCode & bc) override;
};

struct ExprDivEq : ExprBinary {
    ExprDivEq(Expr * a, Expr * b, uint line);
    void Compile(ByteCode & bc) override;
};

struct ExprPowEq : ExprBinary {
    ExprPowEq(Expr * a, Expr * b, uint line);
    void Compile(ByteCode & bc) override;
};

struct ExprIf : Expr {
    Expr * condition{};
    std::vector<Expr*> trueBranch;
    std::vector<Expr*> falseBranch;
    explicit ExprIf(uint line);
    void Compile(ByteCode & bc) override;
};

struct ExprFor : Expr {
    Expr * condition {};
    std::vector<Expr*> expressions;
    explicit ExprFor(uint line);
    void Compile(ByteCode & bc) override;
};

#endif //VIRGO_EXPR_H
