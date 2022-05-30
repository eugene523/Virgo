#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <chrono>
#include <utility>

#include "VM.h"
#include "None.h"
#include "Error.h"
#include "Bool.h"
#include "Int.h"
#include "Real.h"
#include "ByteCode.h"

void Init() {
    VM::Init();
}

/*
void runScript() {
    Init();
    std::fstream f;
    //f.open(R"(C:\code\projects\Virgo\Tests\for.v)");
    f.open(R"(C:\code\projects\Virgo\Tests\first.v)");
    std::stringstream s;
    s << f.rdbuf();
    std::string sourceCode = s.str();
    Parser p;
    p.Interpret(sourceCode);
    VM::contextStack.Print();
    VM::PrintStatus();
}
 */

int main() {
    Init();
    return 0;
}