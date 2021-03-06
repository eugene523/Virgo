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
    f.open(R"(C:\code\Virgo\Tests\arithmetics_int_vars.v)");
    //f.open(R"(C:\code\Virgo\Tests\logics.v)");
    //f.open(R"(C:\code\Virgo\Tests\2_if.v)");
    //f.open(R"(C:\code\Virgo\Tests\test.v)");
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


    std::cout << "\n----------------------";
    script->PrintByteCode();
    std::cout << "\n----------------------";


    script->Execute();
    VM::PrintFrames();
}

#endif // VIRGO_TESTING_H
