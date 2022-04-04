#include <iostream>
#include <fstream>
#include <sstream>
#include "Bool.h"
#include "Mem.h"
#include "VM.h"
#include "Tokenizer.h"
#include "Parser.h"

void test() {
    VM::Init();
}

void runScript() {
    VM::Init();
    std::fstream f;
    //f.open(R"(C:\code\projects\Virgo\tests\for.v)");
    f.open(R"(C:\code\projects\Virgo\tests\first.v)");
    std::stringstream s;
    s << f.rdbuf();
    std::string sourceCode = s.str();
    Parser p;
    p.Interpret(sourceCode);
    VM::contextStack.Print();
    VM::PrintStatus();
}

int main() {
    runScript();
    return 0;
}