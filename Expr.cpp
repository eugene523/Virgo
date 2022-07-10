#include <cassert>
#include <iostream>
#include "Expr.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

Expr::Expr(ExprType exprType, uint line):
exprType{exprType}, line{line} {}

Expr * Expr::GetParentOfType(ExprType parentType) {
    auto * parent = parentExpr;
    while (parent != nullptr) {
        if (parent->exprType == parentType)
            return parent;
        parent = parent->parentExpr;
    }
    return nullptr;
}

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

ExprPushConstant::ExprPushConstant(uint id, uint line):
Expr(ExprType::PushConstant, line), id{id} {}

void ExprPushConstant::Compile(ByteCode & bc) {
    bc.Write_Line(line);
    bc.Write_PushConstant(id);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprEqual::ExprEqual(Expr * a, Expr * b, uint line):
ExprBinary{ExprType::Equal, a, b, line} {}

void ExprEqual::Compile(ByteCode & bc) {
    bc.Write_Line(line);
    a->Compile(bc);
    b->Compile(bc);
    bc.Write<OpCode>(OpCode::Equal);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprNotEqual::ExprNotEqual(Expr * a, Expr * b, uint line):
ExprBinary{ExprType::NotEqual, a, b, line} {}

void ExprNotEqual::Compile(ByteCode & bc) {
    bc.Write_Line(line);
    a->Compile(bc);
    b->Compile(bc);
    bc.Write<OpCode>(OpCode::NotEqual);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprNegate::ExprNegate(Expr * a, uint line):
ExprUnary{ExprType::Negate, a, line} {}

void ExprNegate::Compile(ByteCode & bc) {
    bc.Write_Line(line);
    a->Compile(bc);
    bc.Write<OpCode>(OpCode::Negate);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprAdd::ExprAdd(Expr * a, Expr * b, uint line):
ExprBinary{ExprType::Add, a, b, line} {}

void ExprAdd::Compile(ByteCode & bc) {
    bc.Write_Line(line);
    a->Compile(bc);
    b->Compile(bc);
    bc.Write<OpCode>(OpCode::Add);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprSubtract::ExprSubtract(Expr * a, Expr * b, uint line):
ExprBinary{ExprType::Subtract, a, b, line} {}

void ExprSubtract::Compile(ByteCode & bc) {
    bc.Write_Line(line);
    a->Compile(bc);
    b->Compile(bc);
    bc.Write<OpCode>(OpCode::Subtract);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprMultiply::ExprMultiply(Expr * a, Expr * b, uint line):
ExprBinary{ExprType::Multiply, a, b, line} {}

void ExprMultiply::Compile(ByteCode & bc) {
    bc.Write_Line(line);
    a->Compile(bc);
    b->Compile(bc);
    bc.Write<OpCode>(OpCode::Multiply);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprDivide::ExprDivide(Expr * a, Expr * b, uint line):
ExprBinary{ExprType::Divide, a, b, line} {}

void ExprDivide::Compile(ByteCode & bc) {
    bc.Write_Line(line);
    a->Compile(bc);
    b->Compile(bc);
    bc.Write<OpCode>(OpCode::Divide);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprPower::ExprPower(Expr * a, Expr * b, uint line):
ExprBinary{ExprType::Power, a, b, line} {}

void ExprPower::Compile(ByteCode & bc) {
    bc.Write_Line(line);
    a->Compile(bc);
    b->Compile(bc);
    bc.Write<OpCode>(OpCode::Power);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprGreater::ExprGreater(Expr * a, Expr * b, uint line):
ExprBinary{ExprType::Greater, a, b, line} {}

void ExprGreater::Compile(ByteCode & bc) {
    bc.Write_Line(line);
    a->Compile(bc);
    b->Compile(bc);
    bc.Write<OpCode>(OpCode::Greater);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprGreaterOrEqual::ExprGreaterOrEqual(Expr * a, Expr * b, uint line):
ExprBinary{ExprType::GreaterOrEqual, a, b, line} {}

void ExprGreaterOrEqual::Compile(ByteCode & bc) {
    bc.Write_Line(line);
    a->Compile(bc);
    b->Compile(bc);
    bc.Write<OpCode>(OpCode::GreaterOrEqual);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprLess::ExprLess(Expr * a, Expr * b, uint line):
ExprBinary{ExprType::Less, a, b, line} {}

void ExprLess::Compile(ByteCode & bc) {
    bc.Write_Line(line);
    a->Compile(bc);
    b->Compile(bc);
    bc.Write<OpCode>(OpCode::Less);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprLessOrEqual::ExprLessOrEqual(Expr * a, Expr * b, uint line):
ExprBinary{ExprType::LessOrEqual, a, b, line} {}

void ExprLessOrEqual::Compile(ByteCode & bc) {
    bc.Write_Line(line);
    a->Compile(bc);
    b->Compile(bc);
    bc.Write<OpCode>(OpCode::LessOrEqual);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprNot::ExprNot(Expr * a, uint line):
ExprUnary{ExprType::Not, a, line} {}

void ExprNot::Compile(ByteCode & bc) {
    bc.Write_Line(line);
    a->Compile(bc);
    bc.Write<OpCode>(OpCode::Not);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprAnd::ExprAnd(Expr * a, Expr * b, uint line):
ExprBinary{ExprType::And, a, b, line} {}

void ExprAnd::Compile(ByteCode & bc) {
    bc.Write_Line(line);
    a->Compile(bc);
    b->Compile(bc);
    bc.Write<OpCode>(OpCode::And);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprOr::ExprOr(Expr * a, Expr * b, uint line):
ExprBinary{ExprType::Or, a, b, line} {}

void ExprOr::Compile(ByteCode & bc) {
    bc.Write_Line(line);
    a->Compile(bc);
    b->Compile(bc);
    bc.Write<OpCode>(OpCode::Or);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprAddAssign::ExprAddAssign(Expr * a, Expr * b, uint line):
ExprBinary{ExprType::AddAssign, a, b, line} {}

void ExprAddAssign::Compile(ByteCode & bc) {
    bc.Write_Line(line);
    a->Compile(bc);
    b->Compile(bc);
    bc.Write<OpCode>(OpCode::Add);
    assert(a->exprType == ExprType::Dot);
    uint id = ((ExprDot*)a)->fieldNameId;
    bc.Write_SetLocalVariable(id);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprSubtractAssign::ExprSubtractAssign(Expr * a, Expr * b, uint line):
ExprBinary(ExprType::SubtractAssign, a, b, line) {}

void ExprSubtractAssign::Compile(ByteCode & bc) {
    bc.Write_Line(line);
    a->Compile(bc);
    b->Compile(bc);
    bc.Write<OpCode>(OpCode::Subtract);
    assert(a->exprType == ExprType::Dot);
    uint id = ((ExprDot*)a)->fieldNameId;
    bc.Write_SetLocalVariable(id);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprMultiplyAssign::ExprMultiplyAssign(Expr * a, Expr * b, uint line):
ExprBinary(ExprType::MultiplyAssign, a, b, line) {}

void ExprMultiplyAssign::Compile(ByteCode & bc) {
    bc.Write_Line(line);
    a->Compile(bc);
    b->Compile(bc);
    bc.Write<OpCode>(OpCode::Multiply);
    assert(a->exprType == ExprType::Dot);
    uint id = ((ExprDot*)a)->fieldNameId;
    bc.Write_SetLocalVariable(id);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprDivideAssign::ExprDivideAssign(Expr * a, Expr * b, uint line):
ExprBinary(ExprType::DivideAssign, a, b, line) {}

void ExprDivideAssign::Compile(ByteCode & bc) {
    bc.Write_Line(line);
    a->Compile(bc);
    b->Compile(bc);
    bc.Write<OpCode>(OpCode::Divide);
    assert(a->exprType == ExprType::Dot);
    uint id = ((ExprDot*)a)->fieldNameId;
    bc.Write_SetLocalVariable(id);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprPowerAssign::ExprPowerAssign(Expr * a, Expr * b, uint line):
ExprBinary(ExprType::PowerAssign, a, b, line) {}

void ExprPowerAssign::Compile(ByteCode & bc) {
    bc.Write_Line(line);
    a->Compile(bc);
    b->Compile(bc);
    bc.Write<OpCode>(OpCode::Power);
    assert(a->exprType == ExprType::Dot);
    uint id = ((ExprDot*)a)->fieldNameId;
    bc.Write_SetLocalVariable(id);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprLabel::ExprLabel(std::string labelName, uint line):
Expr(ExprType::Label, line), labelName{labelName} {}

void ExprLabel::Compile(ByteCode & bc) {
    auto * labelAggregator = GetParentOfType(ExprType::Script);
    assert(labelAggregator != nullptr);
    uint labelPos = bc.pos;
    ((ExprScript*)labelAggregator)->AddLabel(labelName, labelPos, line);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprJump::ExprJump(std::string labelName, uint line):
Expr(ExprType::Jump, line), labelName{labelName} {}

void ExprJump::Compile(ByteCode & bc) {
    bc.Write_Line(line);
    pos_Jump = bc.Reserve_OpCode_OpArg();
}

void ExprJump::Correct(ByteCode & bc) {
    auto * labelAggregator = GetParentOfType(ExprType::Script);
    assert(labelAggregator != nullptr);
    uint jumpToPos = ((ExprScript*)labelAggregator)->GetLabelPos(labelName, line);
    bc.Write_OpCode_OpArg_AtPos(pos_Jump, OpCode::Jump, jumpToPos);
}

void CorrectJumpsRecursive(std::vector<Expr*> & expressions, ByteCode & bc) {
    for (size_t i = 0; i < expressions.size(); i++)
    {
        auto * expr = expressions[i];
        switch (expr->exprType)
        {
            case ExprType::Jump:
                ((ExprJump*)expr)->Correct(bc);
                break;

            case ExprType::If:
                ((ExprIf*)expr)->CorrectJumps(bc);
                break;

            case ExprType::For:
                ((ExprFor*)expr)->CorrectJumps(bc);
                break;

            default:
                break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprBreak::ExprBreak(uint line):
Expr(ExprType::Break, line) {}

void ExprBreak::Compile(ByteCode & bc) {
    bc.Write_Line(line);
    pos_Jump = bc.Reserve_OpCode_OpArg();
}

void ExprBreak::Correct(ByteCode & bc) {
    auto * exprFor = (ExprFor*)GetParentOfType(ExprType::For);
    if (exprFor == nullptr) {
        std::cerr << "Syntax error. Line " << line << ". "
                  << "Can't find outer 'for' loop for a 'break' statement.";
        abort();
    }
    bc.Write_OpCode_OpArg_AtPos(pos_Jump, OpCode::Jump, exprFor->pos_AfterFor);
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

ExprSkip::ExprSkip(uint line):
Expr(ExprType::Skip, line) {}

void ExprSkip::Compile(ByteCode & bc) {
    bc.Write_Line(line);
    pos_Jump = bc.Reserve_OpCode_OpArg();
}

void ExprSkip::Correct(ByteCode & bc) {
    // Get parent of ExprFor type
    auto * exprFor = (ExprFor*)GetParentOfType(ExprType::For);
    if (exprFor == nullptr) {
        std::cerr << "Syntax error. Line " << line
                  << ". Can't find outer 'for' loop for a 'skip' statement.";
        abort();
    }

    if (exprFor->pos_StartIteration == 0) {
        std::cerr << "Syntax error. Line " << line
                  << ". Can't find iteration code in outer 'for' loop for a 'skip' statement.";
        abort();
    }
    bc.Write_OpCode_OpArg_AtPos(pos_Jump, OpCode::Jump, exprFor->pos_StartIteration);
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
/*

Variant 1: No false-branch.

Code:
    if condition
        true-branch-code

Diagram:
                        --------------------------------
                        |
                        | CONDITION CODE
                        |
  pos_AfterCondition -> --------------------------------
                        | JumpIfFalse pos_AfterIf
                        --------------------------------
                        |
                        | TRUE BRANCH CODE
                        |
         pos_AfterIf -> --------------------------------
                        |
                        | AFTER 'if' CODE
                        |
                        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*********************************************************************

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
                        | JumpIfFalse pos_StartFalseBranch
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
                        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/

    bc.Write_Line(line);

    // Compiling condition
    condition->Compile(bc);

    // Reserving space for JumpIfFalse instruction
    uint pos_AfterCondition = bc.Reserve_OpCode_OpArg();

    // Compiling true-branch
    for (size_t i = 0; i < trueBranch.size(); i++) {
        trueBranch[i]->Compile(bc);
    }

    if (falseBranch.empty()) {
        uint pos_AfterIf = bc.pos;
        bc.Write_OpCode_OpArg_AtPos(pos_AfterCondition, OpCode::JumpIfFalse, pos_AfterIf);
        return;
    }

    // Reserving space for Jump instruction
    uint pos_AfterTrueBranch = bc.Reserve_OpCode_OpArg();
    uint pos_StartFalseBranch = bc.pos;
    bc.Write_OpCode_OpArg_AtPos(pos_AfterCondition, OpCode::JumpIfFalse, pos_StartFalseBranch);

    // Compiling false-branch
    for (size_t i = 0; i < falseBranch.size(); i++) {
        falseBranch[i]->Compile(bc);
    }

    uint pos_AfterIf = bc.pos;
    bc.Write_OpCode_OpArg_AtPos(pos_AfterTrueBranch, OpCode::Jump, pos_AfterIf);
}

void ExprIf::CorrectJumps(ByteCode & bc) {
    std::vector <Expr*> all;
    all.insert(all.end(), trueBranch.begin(), trueBranch.end());
    all.insert(all.end(), falseBranch.begin(), falseBranch.end());
    CorrectJumpsRecursive(all, bc);
}

void ExprIf::CorrectBreaks(ByteCode & bc) {
    std::vector <Expr*> all;
    all.insert(all.end(), trueBranch.begin(), trueBranch.end());
    all.insert(all.end(), falseBranch.begin(), falseBranch.end());
    CorrectBreaksRecursive(all, bc);
}

void ExprIf::CorrectSkips(ByteCode & bc) {
    std::vector <Expr*> all;
    all.insert(all.end(), trueBranch.begin(), trueBranch.end());
    all.insert(all.end(), falseBranch.begin(), falseBranch.end());
    CorrectSkipsRecursive(all, bc);
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
                        | JumpIfFalse pos_AfterFor
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

    uint pos_StartCondition = bc.pos;

    // Compiling condition
    condition->Compile(bc);

    // Reserving space for JumpIfFalse instruction
    uint pos_AfterCondition = bc.Reserve_OpCode_OpArg();

    // Compiling loop body expressions
    for (size_t i = 0; i < body.size(); i++) {
        body[i]->Compile(bc);
    }

    bc.Write_Jump(pos_StartCondition);

    pos_AfterFor = bc.pos;
    bc.Write_OpCode_OpArg_AtPos(pos_AfterCondition, OpCode::JumpIfFalse, pos_AfterFor);
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
                        | JumpIfFalse pos_AfterFor
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

    uint pos_StartCondition = bc.pos;

    // Compiling condition
    condition->Compile(bc);

    // Reserving space for JumpIfFalse instruction
    uint pos_AfterCondition = bc.Reserve_OpCode_OpArg();

    // Compiling loop body expressions
    for (size_t i = 0; i < body.size(); i++) {
        body[i]->Compile(bc);
    }

    if (iter.size() > 0) {
        pos_StartIteration = bc.pos;
        for (size_t i = 0; i < iter.size(); i++) {
            iter[i]->Compile(bc);
        }
    }

    bc.Write_Jump(pos_StartCondition);

    pos_AfterFor = bc.pos;
    bc.Write_OpCode_OpArg_AtPos(pos_AfterCondition, OpCode::JumpIfFalse, pos_AfterFor);
}

void ExprFor::CorrectJumps(ByteCode & bc) {
    CorrectJumpsRecursive(body, bc);
}

void ExprFor::CorrectBreaks(ByteCode & bc) {
    CorrectBreaksRecursive(body, bc);
}

void ExprFor::CorrectSkips(ByteCode & bc) {
    CorrectSkipsRecursive(body, bc);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprDot::ExprDot(uint fieldNameId, uint line) :
Expr(ExprType::Dot, line), fieldNameId{fieldNameId} {}

ExprDot::ExprDot(Expr * target, uint fieldNameId, uint line) :
Expr(ExprType::Dot, line), target{target}, fieldNameId{fieldNameId} {}

void ExprDot::SetValueExpr(Expr * value_) {
    value = value_;
    isAssignment = true;
}

void ExprDot::Compile(ByteCode & bc) {
    bc.Write_Line(line);
    if (!isAssignment) {
        if (target == nullptr) {
            bc.Write_GetLocalVariable(fieldNameId);
            return;
        } else {
            /*
            target->Compile(bc);
            bc.Write_GetField(fieldNameId);
            */
        }
        return;
    }

    if (target == nullptr) {
        value->Compile(bc);
        bc.Write_SetLocalVariable(fieldNameId);
    } else {
        target->Compile(bc);
        value->Compile(bc);
        /*
        bc.Write_SetField(fieldNameId);
        */
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprArgs::ExprArgs(uint line):
Expr(ExprType::Args, line) {}

void ExprArgs::AddArgExpr(Expr * arg) {
    args.push_back(arg);
}

void ExprArgs::Compile(ByteCode & bc) {
    uint numOfArgs = args.size();
    for (uint i = 0; i < numOfArgs; i++) {
        args[i]->Compile(bc);
    }
    bc.Write_PushInt32((int32_t)numOfArgs);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprAssert::ExprAssert(Expr * checkingExpr, uint line, uint lineId, uint messageId) :
Expr(ExprType::Assert, line), checkingExpr{checkingExpr}, lineId{lineId}, messageId{messageId} {}

void ExprAssert::Compile(ByteCode & bc) {
    bc.Write_Line(line);
    checkingExpr->Compile(bc);
    bc.Write_PushConstant(lineId);
    bc.Write_PushConstant(messageId);
    bc.Write<OpCode>(OpCode::Assert);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

ExprScript::ExprScript() :
Expr(ExprType::Script, 0) {}

void ExprScript::AddExpr(Expr * expr) {
    expressions.push_back(expr);
    expr->parentExpr = this;
}

void ExprScript::AddLabel(std::string labelName, uint labelPos, uint labelLine) {
    if (labels.count(labelName) > 0) {
        std::cout << "Syntax Error. Line " << labelLine
                  << ". Redefinition of '" << labelName << "' label.";
        abort();
    }
    labels[labelName] = labelPos;
}

uint ExprScript::GetLabelPos(std::string labelName, uint jumpLine) {
    if (labels.count(labelName) == 0) {
        std::cout << "Syntax Error. Line " << jumpLine
                  << ". No such label '" << labelName << "'.";
        abort();
    }
    return labels[labelName];
}

void ExprScript::Compile(ByteCode & bc) {
    bc.Write_NewContext();
    for (size_t i = 0; i < expressions.size(); i++) {
        expressions[i]->Compile(bc);
    }
    //bc.Write_CloseContext();
    CorrectJumpsRecursive(expressions, bc);
    CorrectBreaksRecursive(expressions, bc);
    CorrectSkipsRecursive(expressions, bc);
}
