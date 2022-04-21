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
    int count = 10000;
    std::uint64_t * nums = new std::uint64_t[count];
    nums[0] = 0;
    nums[1] = 1;
    nums[2] = 2;
    printf("%p : %i\n", nums, *nums);
    printf("%p : %i\n", nums + 1, *(nums + 1));
    printf("%p : %i\n", nums + 2, *(nums + 2));
    printf("%p\n", nums);
    std::uint64_t * newAddr = (std::uint64_t*)((((std::uintptr_t)nums >> 12) + 1) << 12);
    printf("%p\n", newAddr);
    return 0;
}