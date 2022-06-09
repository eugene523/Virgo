#include "Expr.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

Expr::Expr(ExprType exprType, uint line) : 
exprType{exprType}, line{line} {}

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

ExprLoadConstant::ExprLoadConstant(uint id, uint line) :
Expr(ExprType::LoadConstant, line), id{id} {}

void ExprLoadConstant::Compile(ByteCode & bc) {
    bc.Write_LoadConstant(id);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprGetLocalVariable::ExprGetLocalVariable(uint id, uint line) :
Expr(ExprType::GetLocalVariable, line), id{id} {}

void ExprGetLocalVariable::Compile(ByteCode & bc) {
    bc.Write_GetLocalVariable(id);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprSetLocalVariable::ExprSetLocalVariable(uint id, Expr * valueExpr, uint line) :
Expr(ExprType::GetLocalVariable, line), valueExpr{valueExpr}, id{id} {}

void ExprSetLocalVariable::Compile(ByteCode & bc) {
    valueExpr->Compile(bc);
    bc.Write_SetLocalVariable(id);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprAdd::ExprAdd(Expr * a, Expr * b, uint line) :
ExprBinary{ExprType::Add, a, b, line} {}

void ExprAdd::Compile(ByteCode & bc) {
    a->Compile(bc);
    b->Compile(bc);
    bc.Write_OpCode(OpCode::Add);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprSub::ExprSub(Expr * a, Expr * b, uint line) :
ExprBinary{ExprType::Sub, a, b, line} {}

void ExprSub::Compile(ByteCode & bc) {
    a->Compile(bc);
    b->Compile(bc);
    bc.Write_OpCode(OpCode::Sub);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprMul::ExprMul(Expr * a, Expr * b, uint line) :
ExprBinary{ExprType::Mul, a, b, line} {}

void ExprMul::Compile(ByteCode & bc) {
    a->Compile(bc);
    b->Compile(bc);
    bc.Write_OpCode(OpCode::Mul);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprDiv::ExprDiv(Expr * a, Expr * b, uint line) :
ExprBinary{ExprType::Div, a, b, line} {}

void ExprDiv::Compile(ByteCode & bc) {
    a->Compile(bc);
    b->Compile(bc);
    bc.Write_OpCode(OpCode::Div);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprPow::ExprPow(Expr * a, Expr * b, uint line) :
ExprBinary{ExprType::Pow, a, b, line} {}

void ExprPow::Compile(ByteCode & bc) {
    a->Compile(bc);
    b->Compile(bc);
    bc.Write_OpCode(OpCode::Pow);
}
