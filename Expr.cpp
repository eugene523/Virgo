#include <cassert>
#include "Expr.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

Expr::Expr(ExprType exprType, uint line):
exprType{exprType}, line{line} {}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprUnary::ExprUnary(ExprType exprType, Expr * a, uint line):
Expr{exprType, line}, a{a} {}

ExprUnary::~ExprUnary() {
    delete a;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprBinary::ExprBinary(ExprType exprType, Expr * a, Expr * b, uint line):
Expr{exprType, line}, a{a}, b{b} {}

ExprBinary::~ExprBinary() {
    delete a;
    delete b;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprLoadConstant::ExprLoadConstant(uint id, uint line):
Expr(ExprType::LoadConstant, line), id{id} {}

void ExprLoadConstant::Compile(ByteCode & bc) {
    bc.Write_Line(line);
    bc.Write_LoadConstant(id);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprGetLocalVariable::ExprGetLocalVariable(uint id, uint line):
Expr(ExprType::GetLocalVariable, line), id{id} {}

void ExprGetLocalVariable::Compile(ByteCode & bc) {
    bc.Write_Line(line);
    bc.Write_GetLocalVariable(id);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprSetLocalVariable::ExprSetLocalVariable(uint id, Expr * valueExpr, uint line):
Expr(ExprType::GetLocalVariable, line), valueExpr{valueExpr}, id{id} {}

void ExprSetLocalVariable::Compile(ByteCode & bc) {
    bc.Write_Line(line);
    valueExpr->Compile(bc);
    bc.Write_SetLocalVariable(id);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprEq::ExprEq(Expr * a, Expr * b, uint line):
ExprBinary{ExprType::Eq, a, b, line} {}

void ExprEq::Compile(ByteCode & bc) {
    bc.Write_Line(line);
    a->Compile(bc);
    b->Compile(bc);
    bc.Write_OpCode(OpCode::Eq);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprNotEq::ExprNotEq(Expr * a, Expr * b, uint line):
ExprBinary{ExprType::NotEq, a, b, line} {}

void ExprNotEq::Compile(ByteCode & bc) {
    bc.Write_Line(line);
    a->Compile(bc);
    b->Compile(bc);
    bc.Write_OpCode(OpCode::NotEq);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprNeg::ExprNeg(Expr * a, uint line) :
ExprUnary{ExprType::Neg, a, line} {}

void ExprNeg::Compile(ByteCode & bc) {
    bc.Write_Line(line);
    a->Compile(bc);
    bc.Write_OpCode(OpCode::Neg);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprAdd::ExprAdd(Expr * a, Expr * b, uint line):
ExprBinary{ExprType::Add, a, b, line} {}

void ExprAdd::Compile(ByteCode & bc) {
    bc.Write_Line(line);
    a->Compile(bc);
    b->Compile(bc);
    bc.Write_OpCode(OpCode::Add);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprSub::ExprSub(Expr * a, Expr * b, uint line):
ExprBinary{ExprType::Sub, a, b, line} {}

void ExprSub::Compile(ByteCode & bc) {
    bc.Write_Line(line);
    a->Compile(bc);
    b->Compile(bc);
    bc.Write_OpCode(OpCode::Sub);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprMul::ExprMul(Expr * a, Expr * b, uint line):
ExprBinary{ExprType::Mul, a, b, line} {}

void ExprMul::Compile(ByteCode & bc) {
    bc.Write_Line(line);
    a->Compile(bc);
    b->Compile(bc);
    bc.Write_OpCode(OpCode::Mul);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprDiv::ExprDiv(Expr * a, Expr * b, uint line):
ExprBinary{ExprType::Div, a, b, line} {}

void ExprDiv::Compile(ByteCode & bc) {
    bc.Write_Line(line);
    a->Compile(bc);
    b->Compile(bc);
    bc.Write_OpCode(OpCode::Div);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprPow::ExprPow(Expr * a, Expr * b, uint line):
ExprBinary{ExprType::Pow, a, b, line} {}

void ExprPow::Compile(ByteCode & bc) {
    bc.Write_Line(line);
    a->Compile(bc);
    b->Compile(bc);
    bc.Write_OpCode(OpCode::Pow);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprGr::ExprGr(Expr * a, Expr * b, uint line) :
ExprBinary{ExprType::Gr, a, b, line} {}

void ExprGr::Compile(ByteCode & bc) {
    bc.Write_Line(line);
    a->Compile(bc);
    b->Compile(bc);
    bc.Write_OpCode(OpCode::Gr);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprGrEq::ExprGrEq(Expr * a, Expr * b, uint line) :
ExprBinary{ExprType::GrEq, a, b, line} {}

void ExprGrEq::Compile(ByteCode & bc) {
    bc.Write_Line(line);
    a->Compile(bc);
    b->Compile(bc);
    bc.Write_OpCode(OpCode::GrEq);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprLs::ExprLs(Expr * a, Expr * b, uint line) :
ExprBinary{ExprType::Ls, a, b, line} {}

void ExprLs::Compile(ByteCode & bc) {
    bc.Write_Line(line);
    a->Compile(bc);
    b->Compile(bc);
    bc.Write_OpCode(OpCode::Ls);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprLsEq::ExprLsEq(Expr * a, Expr * b, uint line) :
ExprBinary{ExprType::LsEq, a, b, line} {}

void ExprLsEq::Compile(ByteCode & bc) {
    bc.Write_Line(line);
    a->Compile(bc);
    b->Compile(bc);
    bc.Write_OpCode(OpCode::LsEq);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprNot::ExprNot(Expr * a, uint line) :
ExprUnary{ExprType::Not, a, line} {}

void ExprNot::Compile(ByteCode & bc) {
    bc.Write_Line(line);
    a->Compile(bc);
    bc.Write_OpCode(OpCode::Not);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprAnd::ExprAnd(Expr * a, Expr * b, uint line) :
ExprBinary{ExprType::And, a, b, line} {}

void ExprAnd::Compile(ByteCode & bc) {
    bc.Write_Line(line);
    a->Compile(bc);
    b->Compile(bc);
    bc.Write_OpCode(OpCode::And);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprOr::ExprOr(Expr * a, Expr * b, uint line) :
ExprBinary{ExprType::Or, a, b, line} {}

void ExprOr::Compile(ByteCode & bc) {
    bc.Write_Line(line);
    a->Compile(bc);
    b->Compile(bc);
    bc.Write_OpCode(OpCode::Or);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprIf::ExprIf(uint line):
Expr{ExprType::If, line} {}

void ExprIf::Compile(ByteCode & bc) {
    bc.Write_Line(line);

    // Compiling condition
    condition->Compile(bc);

    // Reserving space for JumpFalse instruction
    uint pos_AfterCondition = bc.Reserve(sizeof(OpCode) + sizeof(uint64_t));

    // Compiling true-branch
    for (size_t i = 0; i < trueBranch.size(); i++) {
        trueBranch[i]->Compile(bc);
    }

    if (falseBranch.empty()) {
        bc.Write_OpCode_uint64_AtPos(pos_AfterCondition, OpCode::JumpFalse, bc.bcPos);
        return;
    }

    // Reserving space for Jump instruction
    uint pos_AfterTrueBranch = bc.Reserve(sizeof(OpCode) + sizeof(uint64_t));
    bc.Write_OpCode_uint64_AtPos(pos_AfterCondition, OpCode::JumpFalse, bc.bcPos);

    // Compiling false-branch
    for (size_t i = 0; i < falseBranch.size(); i++) {
        falseBranch[i]->Compile(bc);
    }
    bc.Write_OpCode_uint64_AtPos(pos_AfterTrueBranch, OpCode::Jump, bc.bcPos);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprAddEq::ExprAddEq(Expr * a, Expr * b, uint line):
ExprBinary{ExprType::AddEq, a, b, line} {}

void ExprAddEq::Compile(ByteCode & bc) {
    bc.Write_Line(line);
    a->Compile(bc);
    b->Compile(bc);
    bc.Write_OpCode(OpCode::Add);
    assert(a->exprType == ExprType::GetLocalVariable);
    uint id = ((ExprGetLocalVariable*)a)->id;
    bc.Write_SetLocalVariable(id);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprSubEq::ExprSubEq(Expr * a, Expr * b, uint line):
ExprBinary(ExprType::SubEq, a, b, line) {}

void ExprSubEq::Compile(ByteCode & bc) {
    bc.Write_Line(line);
    a->Compile(bc);
    b->Compile(bc);
    bc.Write_OpCode(OpCode::Sub);
    assert(a->exprType == ExprType::GetLocalVariable);
    uint id = ((ExprGetLocalVariable*)a)->id;
    bc.Write_SetLocalVariable(id);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprMulEq::ExprMulEq(Expr * a, Expr * b, uint line):
ExprBinary(ExprType::MulEq, a, b, line) {}

void ExprMulEq::Compile(ByteCode & bc) {
    bc.Write_Line(line);
    a->Compile(bc);
    b->Compile(bc);
    bc.Write_OpCode(OpCode::Mul);
    assert(a->exprType == ExprType::GetLocalVariable);
    uint id = ((ExprGetLocalVariable*)a)->id;
    bc.Write_SetLocalVariable(id);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprDivEq::ExprDivEq(Expr * a, Expr * b, uint line):
ExprBinary(ExprType::DivEq, a, b, line) {}

void ExprDivEq::Compile(ByteCode & bc) {
    bc.Write_Line(line);
    a->Compile(bc);
    b->Compile(bc);
    bc.Write_OpCode(OpCode::Div);
    assert(a->exprType == ExprType::GetLocalVariable);
    uint id = ((ExprGetLocalVariable*)a)->id;
    bc.Write_SetLocalVariable(id);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprPowEq::ExprPowEq(Expr * a, Expr * b, uint line):
ExprBinary(ExprType::PowEq, a, b, line) {}

void ExprPowEq::Compile(ByteCode & bc) {
    bc.Write_Line(line);
    a->Compile(bc);
    b->Compile(bc);
    bc.Write_OpCode(OpCode::Pow);
    assert(a->exprType == ExprType::GetLocalVariable);
    uint id = ((ExprGetLocalVariable*)a)->id;
    bc.Write_SetLocalVariable(id);
}
