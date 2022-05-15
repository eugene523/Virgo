#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <chrono>
//#include <utility>
#include "Mem.h"
#include "Error.h"
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
#include "Error.h"
#include "Bool.h"
#include "Int.h"
#include "Real.h"

void Init() {
    Heap::Init();
    None::InitType();
    Error::InitType();
    Bool::InitType();
    Int::InitType();
    Real::InitType();
}

std::byte * fakeChunk(MemDomain & m, uint size) {
    std::byte * chunk = m.GetChunk(size);
    Obj::Init(chunk, None::t);
    return chunk;
}

uint randomChunkSize() {
    return (24 + rand() % (64 - 24 + 1));
}

void MemDomain_Test1() {
    const uint numOfObjects = 1000;
    MemDomain m = MemDomain();
    std::list<std::byte*> chunks;
    for (uint i = 0; i < numOfObjects; i++) {
        uint size = randomChunkSize();
        chunks.push_back(fakeChunk(m, size));
    }
    assert(m.NumOfObj() == numOfObjects);
    for (auto c : chunks)
        Page::FreeChunk(c);
    assert(m.NumOfObj() == 0);
}

int main() {
    Init();
    MemDomain m = MemDomain();
    m.PrintStatus("Before allocation");
    std::list<std::byte*> chunks;
    for (int i = 0; i < 16900; i++) {
        chunks.push_back(fakeChunk(m, randomChunkSize()));
    }
    m.PrintStatus("After allocation");
    m.Gc();
    m.PrintStatus("After gc.");

    return 0;
}