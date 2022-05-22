#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <chrono>
//#include <utility>
#include "Mem.h"
#include "Error.h"

/*
void runScript() {
    VM::Init();
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

std::byte * fakeChunk(MemDomain & m, uint size) {
    std::byte * chunk = m.GetChunk(size);
    Obj::Init(chunk, None::t);
    return chunk;
}

uint randomChunkSize() {
    return (24 + rand() % (64 - 24 + 1));
}

int main() {
    Init();

    ByteCode bc;
    bc.Write_OpCode(OpCode::NewFrame);
    bc.Write_OpCode(OpCode::CloseFrame);
    VM::Execute(bc);

    return 0;
}