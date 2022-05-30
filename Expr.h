#ifndef VIRGO_EXPR_H
#define VIRGO_EXPR_H

#include "Common.h"
#include "ByteCode.h"

enum class ExprType
{
    Undefined,
    LoadConstant,
    GetLocalVariable,
    SetLocalVariable,
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
    explicit ExprSetLocalVariable(uint id, uint line);
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

#endif //VIRGO_EXPR_H
