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
    PushConstant,
    Equal,
    NotEqual,
    Negate,
    Add,
    Subtract,
    Multiply,
    Divide,
    Power,
    Greater,
    GreaterOrEqual,
    Less,
    LessOrEqual,
    Not,
    And,
    Or,
    Label,
    Jump,
    Skip,
    Break,
    Return,
    If,
    For,
    Dot,
    Args,
    Assign,
    AddAssign,
    SubtractAssign,
    MultiplyAssign,
    DivideAssign,
    PowerAssign,
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

struct ExprPushConstant : Expr {
    uint id;
    ExprPushConstant(uint id, uint line);
    void Compile(ByteCode & bc) override;
};

struct ExprEqual : ExprBinary {
    ExprEqual(Expr * a, Expr * b, uint line);
    void Compile(ByteCode & bc) override;
};

struct ExprNotEqual : ExprBinary {
    ExprNotEqual(Expr * a, Expr * b, uint line);
    void Compile(ByteCode & bc) override;
};

struct ExprNegate : ExprUnary {
    ExprNegate(Expr * a, uint line);
    void Compile(ByteCode & bc) override;
};

struct ExprAdd : ExprBinary {
    ExprAdd(Expr * a, Expr * b, uint line);
    void Compile(ByteCode & bc) override;
};

struct ExprSubtract : ExprBinary {
    ExprSubtract(Expr * a, Expr * b, uint line);
    void Compile(ByteCode & bc) override;
};

struct ExprMultiply : ExprBinary {
    ExprMultiply(Expr * a, Expr * b, uint line);
    void Compile(ByteCode & bc) override;
};

struct ExprDivide : ExprBinary {
    ExprDivide(Expr * a, Expr * b, uint line);
    void Compile(ByteCode & bc) override;
};

struct ExprPower : ExprBinary {
    ExprPower(Expr * a, Expr * b, uint line);
    void Compile(ByteCode & bc) override;
};

struct ExprGreater : ExprBinary {
    ExprGreater(Expr * a, Expr * b, uint line);
    void Compile(ByteCode & bc) override;
};

struct ExprGreaterOrEqual : ExprBinary {
    ExprGreaterOrEqual(Expr * a, Expr * b, uint line);
    void Compile(ByteCode & bc) override;
};

struct ExprLess : ExprBinary {
    ExprLess(Expr * a, Expr * b, uint line);
    void Compile(ByteCode & bc) override;
};

struct ExprLessOrEqual : ExprBinary {
    ExprLessOrEqual(Expr * a, Expr * b, uint line);
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

struct ExprAddAssign : ExprBinary {
    ExprAddAssign(Expr * a, Expr * b, uint line);
    void Compile(ByteCode & bc) override;
};

struct ExprSubtractAssign : ExprBinary {
    ExprSubtractAssign(Expr * a, Expr * b, uint line);
    void Compile(ByteCode & bc) override;
};

struct ExprMultiplyAssign : ExprBinary {
    ExprMultiplyAssign(Expr * a, Expr * b, uint line);
    void Compile(ByteCode & bc) override;
};

struct ExprDivideAssign : ExprBinary {
    ExprDivideAssign(Expr * a, Expr * b, uint line);
    void Compile(ByteCode & bc) override;
};

struct ExprPowerAssign : ExprBinary {
    ExprPowerAssign(Expr * a, Expr * b, uint line);
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
    ForIn
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

struct ExprDot : Expr {
    Expr * target{};
    uint fieldNameId;
    Expr * value{};
    bool isAssignment{};

    ExprDot(uint fieldNameId, uint line);
    ExprDot(Expr * target, uint fieldNameId, uint line);
    void SetValueExpr(Expr * value_);
    void Compile(ByteCode & bc) override;
};

struct ExprArgs : Expr {
    std::vector<Expr*> args;
    ExprArgs(uint line);
    void AddArgExpr(Expr * arg);
    void Compile(ByteCode & bc) override;
};

struct ExprAssert : Expr {
    Expr * checkingExpr;
    uint lineId;
    uint messageId;

    ExprAssert(Expr * checkingExpr, uint line, uint lineId, uint messageId);
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
