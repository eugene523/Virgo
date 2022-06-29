#ifndef VIRGO_EXPR_H
#define VIRGO_EXPR_H

#include <string>
#include <vector>
#include <map>
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
    Label,
    Jump,
    Skip,
    Break,
    Return,
    If,
    For,
    ForC,
    ForIn,
    Call,
    Define,
    Assert,
    Script,
};

struct Expr {
    const ExprType exprType;
    const uint line;
    Expr * parentExpr;

    Expr(ExprType exprType, uint line);
    Expr * GetParentOfType(ExprType parentType);
    virtual void Compile(ByteCode & bc) = 0;
    virtual ~Expr() = default;
};

struct ExprUnary : Expr {
    Expr * a;
    ExprUnary(ExprType exprType, Expr * a, uint line);
    ~ExprUnary() override;
};

struct ExprBinary : Expr {
    Expr * a;
    Expr * b;
    ExprBinary(ExprType exprType, Expr * a, Expr * b, uint line);
    ~ExprBinary() override;
};

struct ExprLoadConstant : Expr {
    uint id;
    ExprLoadConstant(uint id, uint line);
    void Compile(ByteCode & bc) override;
};

struct ExprGetLocalVariable : Expr {
    uint id;
    explicit ExprGetLocalVariable(uint id, uint line);
    void Compile(ByteCode & bc) override;
};

struct ExprSetLocalVariable : Expr {
    uint id;
    Expr * valueExpr;
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

struct ExprLabel : Expr {
    std::string labelName;
    ExprLabel(std::string labelName, uint line);
    void Compile(ByteCode & bc) override;
};

struct ExprJump : Expr {
    std::string labelName;
    uint pos_Jump{};
    ExprJump(std::string labelName, uint line);
    void Compile(ByteCode & bc) override;
    void Correct(ByteCode & bc);
};

struct ExprBreak : Expr {
    uint pos_Jump{};
    explicit ExprBreak(uint line);
    void Compile(ByteCode & bc) override;
    void Correct(ByteCode & bc);
};

struct ExprSkip : Expr {
    uint pos_Jump{};
    explicit ExprSkip(uint line);
    void Compile(ByteCode & bc) override;
    void Correct(ByteCode & bc);
};

struct ExprIf : Expr {
    Expr * condition{};
    std::vector<Expr*> trueBranch;
    std::vector<Expr*> falseBranch;

    explicit ExprIf(uint line);
    void SetCondition(Expr * expr);
    void AddTrueExpr(Expr * expr);
    void AddFalseExpr(Expr * expr);
    void Compile(ByteCode & bc) override;
    void CorrectJumps(ByteCode & bc);
    void CorrectBreaks(ByteCode & bc);
    void CorrectSkips(ByteCode & bc);
};

enum class ForType
{
    Undefined,
    Ordinary,
    CStyled,
    Foreach
};

struct ExprFor : Expr {
    ForType            forType{};
    std::vector<Expr*> init;
    Expr *             condition{};
    std::vector<Expr*> iter;
    std::vector<Expr*> body;
    uint               pos_AfterFor{};
    uint               pos_StartIteration{};

    explicit ExprFor(uint line);
    void AddInitExpr(Expr * expr);
    void SetCondition(Expr * expr);
    void AddIterExpr(Expr * expr);
    void AddBodyExpr(Expr * expr);
    void Compile(ByteCode & bc) override;
    void Compile_Ordinary(ByteCode & bc);
    void Compile_CStyled(ByteCode & bc);
    void CorrectJumps(ByteCode & bc);
    void CorrectBreaks(ByteCode & bc);
    void CorrectSkips(ByteCode & bc);
};

struct ExprAssert : Expr {
    Expr * checkingExpr{};
    uint lineId;
    uint messageId;

    ExprAssert(uint line, uint lineId, uint messageId);
    void Compile(ByteCode & bc) override;
};

struct ExprScript : Expr {
    std::vector<Expr*> expressions;
    std::map<std::string, uint> labels;

    explicit ExprScript();
    void AddExpr(Expr * expr);
    void AddLabel(std::string labelName, uint labelPos, uint labelLine);
    uint GetLabelPos(std::string labelName, uint jumpLine);
    void Compile(ByteCode & bc) override;
};

#endif //VIRGO_EXPR_H
