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

int main() {
    Heap::Init();
    None::InitType();
    Err::InitType();
    Bool::InitType();
    Int::InitType();
    Real::InitType();

    Err * err = Err::New("Hello World!", 0);
    std::cout << IS_OF_TYPE(err, Err::t);
    return 0;
}