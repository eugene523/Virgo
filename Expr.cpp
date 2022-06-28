#include <cassert>
#include <iostream>
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
Expr(ExprType::GetLocalVariable, line), id{id}, valueExpr{valueExpr} {}

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

ExprAddEq::ExprAddEq(Expr * a, Expr * b, uint line) :
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

ExprSubEq::ExprSubEq(Expr * a, Expr * b, uint line) :
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

ExprMulEq::ExprMulEq(Expr * a, Expr * b, uint line) :
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

ExprDivEq::ExprDivEq(Expr * a, Expr * b, uint line) :
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

ExprPowEq::ExprPowEq(Expr * a, Expr * b, uint line) :
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

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprBreak::ExprBreak(uint line) :
Expr(ExprType::Break, line) {}

void ExprBreak::Compile(ByteCode & bc) {
    bc.Write_Line(line);
    pos_Jump = bc.Reserve(sizeof(OpCode) + sizeof(uint64_t));
}

void ExprBreak::Correct(ByteCode & bc) {
    // Get parent of ExprFor type
    auto * currentParentExpr = parentExpr;
    while (currentParentExpr != nullptr) {
        if (currentParentExpr->exprType == ExprType::For)
            break;
        currentParentExpr = currentParentExpr->parentExpr;
    }

    if (currentParentExpr == nullptr) {
        std::cerr << "Syntax error. Line " << line << ". "
                  << "Can't find outer 'for' loop for a 'break' statement.";
        abort();
    }

    auto * exprFor = (ExprFor*)currentParentExpr;
    bc.Write_OpCode_uint64_AtPos(pos_Jump, OpCode::Jump, exprFor->pos_AfterFor);
}

void CorrectBreaksRecursive(std::vector<Expr*> & expressions, ByteCode & bc) {
    for (size_t i = 0; i < expressions.size(); i++)
    {
        auto * expr = expressions[i];
        switch (expr->exprType)
        {
            case ExprType::Break:
                ((ExprBreak*)expr)->Correct(bc);
                break;

            case ExprType::If:
                ((ExprIf*)expr)->CorrectBreaks(bc);
                break;

            case ExprType::For:
                ((ExprFor*)expr)->CorrectBreaks(bc);
                break;

            default:
                break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprSkip::ExprSkip(uint line) :
Expr(ExprType::Skip, line) {}

void ExprSkip::Compile(ByteCode & bc) {
    bc.Write_Line(line);
    pos_Jump = bc.Reserve(sizeof(OpCode) + sizeof(uint64_t));
}

void ExprSkip::Correct(ByteCode & bc) {
    // Get parent of ExprFor type
    auto * currentParentExpr = parentExpr;
    while (currentParentExpr != nullptr) {
        if (currentParentExpr->exprType == ExprType::For)
            break;
        currentParentExpr = currentParentExpr->parentExpr;
    }

    if (currentParentExpr == nullptr) {
        std::cerr << "Syntax error. Line " << line << ". "
                  << "Can't find outer 'for' loop for a 'skip' statement.";
        abort();
    }

    auto * exprFor = (ExprFor*)currentParentExpr;
    if (exprFor->pos_StartIteration == 0) {
        std::cerr << "Syntax error. Line " << line << ". "
                  << "Can't find iteration code in outer 'for' loop for a 'skip' statement.";
        abort();
    }
    bc.Write_OpCode_uint64_AtPos(pos_Jump, OpCode::Jump, exprFor->pos_StartIteration);
}

void CorrectSkipsRecursive(std::vector<Expr*> & expressions, ByteCode & bc) {
    for (size_t i = 0; i < expressions.size(); i++)
    {
        auto * expr = expressions[i];
        switch (expr->exprType)
        {
            case ExprType::Skip:
                ((ExprSkip*)expr)->Correct(bc);
                break;

            case ExprType::If:
                ((ExprIf*)expr)->CorrectSkips(bc);
                break;

            case ExprType::For:
                ((ExprFor*)expr)->CorrectSkips(bc);
                break;

            default:
                break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprIf::ExprIf(uint line):
Expr{ExprType::If, line} {}

void ExprIf::SetCondition(Expr * expr) {
    condition = expr;
    condition->parentExpr = this;
}

void ExprIf::AddTrueExpr(Expr * expr) {
    trueBranch.push_back(expr);
    expr->parentExpr = this;
}

void ExprIf::AddFalseExpr(Expr * expr) {
    falseBranch.push_back(expr);
    expr->parentExpr = this;
}

void ExprIf::Compile(ByteCode & bc) {
/**********************************************************

Variant 1: No false-branch.

Code:
    if condition
        true-branch-code

Diagram:
                        -------------------------------
                        |
                        | CONDITION CODE
                        |
  pos_AfterCondition -> --------------------------------
                        | JumpFalse pos_AfterIf
                        --------------------------------
                        |
                        | TRUE BRANCH CODE
                        |
         pos_AfterIf -> --------------------------------
                        |
                        | AFTER 'if' CODE
                        |

***********************************************************

Variant 2: Has false-branch.

Code:
    if condition
        true-branch
    else
        false-branch

Diagram:
                        -------------------------------
                        |
                        | CONDITION CODE
                        |
  pos_AfterCondition -> --------------------------------
                        | JumpFalse pos_StartFalseBranch
                        --------------------------------
                        |
                        | TRUE BRANCH CODE
                        |
 pos_AfterTrueBranch -> --------------------------------
                        | Jump pos_AfterIf
pos_StartFalseBranch -> --------------------------------
                        |
                        | FALSE BRANCH CODE
                        |
         pos_AfterIf -> --------------------------------
                        |
                        | AFTER 'if' CODE
                        |

**********************************************************/

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
        uint pos_AfterIf = bc.bcPos;
        bc.Write_OpCode_uint64_AtPos(pos_AfterCondition, OpCode::JumpFalse, pos_AfterIf);
        return;
    }

    // Reserving space for Jump instruction
    uint pos_AfterTrueBranch = bc.Reserve(sizeof(OpCode) + sizeof(uint64_t));
    uint pos_StartFalseBranch = bc.bcPos;
    bc.Write_OpCode_uint64_AtPos(pos_AfterCondition, OpCode::JumpFalse, pos_StartFalseBranch);

    // Compiling false-branch
    for (size_t i = 0; i < falseBranch.size(); i++) {
        falseBranch[i]->Compile(bc);
    }

    uint pos_AfterIf = bc.bcPos;
    bc.Write_OpCode_uint64_AtPos(pos_AfterTrueBranch, OpCode::Jump, pos_AfterIf);
}

void ExprIf::CorrectBreaks(ByteCode & bc) {
    std::vector <Expr*> allExpressions;
    allExpressions.insert(allExpressions.end(), trueBranch.begin(), trueBranch.end());
    allExpressions.insert(allExpressions.end(), falseBranch.begin(), falseBranch.end());
    CorrectBreaksRecursive(allExpressions, bc);
}

void ExprIf::CorrectSkips(ByteCode & bc) {
    std::vector <Expr*> allExpressions;
    allExpressions.insert(allExpressions.end(), trueBranch.begin(), trueBranch.end());
    allExpressions.insert(allExpressions.end(), falseBranch.begin(), falseBranch.end());
    CorrectSkipsRecursive(allExpressions, bc);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprFor::ExprFor(uint line):
Expr{ExprType::For, line} {}

void ExprFor::AddInitExpr(Expr * expr) {
    init.push_back(expr);
    expr->parentExpr = expr;
}

void ExprFor::SetCondition(Expr * expr) {
    condition = expr;
    condition->parentExpr = this;
}

void ExprFor::AddIterExpr(Expr * expr) {
    iter.push_back(expr);
    expr->parentExpr = expr;
}

void ExprFor::AddBodyExpr(Expr * expr) {
    body.push_back(expr);
    expr->parentExpr = this;
}

void ExprFor::Compile(ByteCode & bc) {
    switch (forType)
    {
        case ForType::Ordinary:
            Compile_Ordinary(bc);
            break;

        case ForType::CStyled:
            Compile_CStyled(bc);
            break;

        default:
            std::cerr << "Unknown type of 'for' loop.";
            abort();
    }
}

void ExprFor::Compile_Ordinary(ByteCode & bc) {
/**********************************************************

Code:
    for condition
        body

Diagram:

  pos_StartCondition -> -------------------------------
                        |
                        | CONDITION CODE
                        |
  pos_AfterCondition -> --------------------------------
                        | JumpFalse pos_AfterFor
                        --------------------------------
                        |
                        | BODY CODE
                        |
                        --------------------------------
                        | Jump pos_StartCondition
        pos_AfterFor -> --------------------------------
                        |
                        | AFTER 'for' CODE
                        |

**********************************************************/

    bc.Write_Line(line);

    uint pos_StartCondition = bc.bcPos;

    // Compiling condition
    condition->Compile(bc);

    // Reserving space for JumpFalse instruction
    uint pos_AfterCondition = bc.Reserve(sizeof(OpCode) + sizeof(uint64_t));

    // Compiling loop body expressions
    for (size_t i = 0; i < body.size(); i++) {
        body[i]->Compile(bc);
    }

    bc.Write_Jump(pos_StartCondition);

    pos_AfterFor = bc.bcPos;
    bc.Write_OpCode_uint64_AtPos(pos_AfterCondition, OpCode::JumpFalse, pos_AfterFor);
}

void ExprFor::Compile_CStyled(ByteCode & bc) {
/**********************************************************

Code:
    for (initialization; condition; iteration)
        body

Diagram:
                        -------------------------------
                        |
                        | INITIALIZATION CODE
                        |
  pos_StartCondition -> -------------------------------
                        |
                        | CONDITION CODE
                        |
  pos_AfterCondition -> --------------------------------
                        | JumpFalse pos_AfterFor
                        --------------------------------
                        |
                        | BODY CODE
                        |
  pos_StartIteration -> --------------------------------
                        |
                        | ITERATION CODE
                        |
                        --------------------------------
                        | Jump pos_StartCondition
        pos_AfterFor -> --------------------------------
                        |
                        | AFTER 'for' CODE
                        |

**********************************************************/

    bc.Write_Line(line);

    for (size_t i = 0; i < init.size(); i++) {
        init[i]->Compile(bc);
    }

    uint pos_StartCondition = bc.bcPos;

    // Compiling condition
    condition->Compile(bc);

    // Reserving space for JumpFalse instruction
    uint pos_AfterCondition = bc.Reserve(sizeof(OpCode) + sizeof(uint64_t));

    // Compiling loop body expressions
    for (size_t i = 0; i < body.size(); i++) {
        body[i]->Compile(bc);
    }

    if (iter.size() > 0) {
        pos_StartIteration = bc.bcPos;
        for (size_t i = 0; i < iter.size(); i++) {
            iter[i]->Compile(bc);
        }
    }

    bc.Write_Jump(pos_StartCondition);

    pos_AfterFor = bc.bcPos;
    bc.Write_OpCode_uint64_AtPos(pos_AfterCondition, OpCode::JumpFalse, pos_AfterFor);
}

void ExprFor::CorrectBreaks(ByteCode & bc) {
    CorrectBreaksRecursive(body, bc);
}

void ExprFor::CorrectSkips(ByteCode & bc) {
    CorrectSkipsRecursive(body, bc);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprScript::ExprScript():
Expr(ExprType::Script, 0) {}

void ExprScript::AddExpr(Expr * expr) {
    expressions.push_back(expr);
    expr->parentExpr = this;
}

void ExprScript::Compile(ByteCode & bc) {
    bc.Write_NewFrame();
    for (size_t i = 0; i < expressions.size(); i++) {
        expressions[i]->Compile(bc);
    }
    //bc.Write_CloseFrame();
}

void ExprScript::CorrectBreaks(ByteCode & bc) {
    CorrectBreaksRecursive(expressions, bc);
}

void ExprScript::CorrectSkips(ByteCode & bc) {
    CorrectSkipsRecursive(expressions, bc);
}
