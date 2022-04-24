#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include "Mem.h"
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
    std::byte * page = (std::byte*)calloc(10000, 1);
    Page::Init(page, nullptr, 32);
    std::byte * c = ((Page*)page)->GetChunk();
    std::cout << ((Page*)page)->NumOfFreeChunks();
    //Page_FreeChunk(c);
    //std::cout << Page_GetNumOfFreeChunks((Page*)page);
    return 0;
}