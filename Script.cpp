#include "Script.h"
#include "VM.h"

Script::Script() = default;

Script::~Script() {
    delete exprScript;
}

void Script::SetExprScript(ExprScript * exprScript_) {
    exprScript = exprScript_;
}

void Script::Compile() {
    exprScript->Compile(bc);
    exprScript->CorrectBreaks(bc);
    exprScript->CorrectSkips(bc);
}

void Script::Execute() {
    VM::Execute(bc);
}

void Script::PrintByteCode() {
    bc.Print();
}
