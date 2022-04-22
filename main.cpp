#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include "Mem.h"
#include "Bool.h"
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

int main() {
    std::byte * addr = (std::byte*)calloc(10000, 1);
    std::byte * page = (std::byte*)(((((std::uint64_t)addr) >> 12) + 1) << 12);
    Page_Init(page, nullptr, 32);
    std::byte * c = Page_GetChunk((PageHeader*)page);
    Page_FreeChunk(c);
    std::cout << Page_GetNumOfFreeChunks((PageHeader*)page);
    return 0;
}