#ifndef VIRGO_TESTING_H
#define VIRGO_TESTING_H

#include "VM.h"
#include "None.h"
#include "Error.h"
#include "Bool.h"
#include "Int.h"
#include "Real.h"
#include "ByteCode.h"
#include "Tokenizer.h"
#include "Parser.h"

void Init() {
    VM::Init();
}

void RunScript() {
    Init();

    std::fstream f;
    f.open(R"(C:\code\Virgo\Tests\1_arithmetics_add.v)");
    std::stringstream s;
    s << f.rdbuf();
    std::string src = s.str();

    Tokenizer tokenizer;
    tokenizer.Tokenize(src);
    if (tokenizer.HasError()) {
        std::cout << tokenizer.GetErrorMessage();
        return;
    }
    VM::PrintConstants();

    Parser p;
    Script * script = p.Parse(tokenizer.GetTokens());
    script->Compile();
    //script->Execute();
    script->PrintByteCode();
    //VM::PrintFrames();
}

#endif // VIRGO_TESTING_H
