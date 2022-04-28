#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <chrono>
//#include <utility>
#include "Mem.h"
#include "Err.h"
/*
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

void Mem2Test() {
    std::cout << sizeof(ObjHnd);
}
*/

#include "None.h"
#include "Err.h"
#include "Bool.h"
#include "Int.h"
#include "Real.h"

void Init() {
    Heap::Init();
    None::InitType();
    Err::InitType();
    Bool::InitType();
    Int::InitType();
    Real::InitType();
}

int main() {
    Init();
    std::byte ** p = (std::byte**)malloc(10);
    std::cout << std::hex << p << '\n';
    std::cout << (p + 1);
    return 0;
}