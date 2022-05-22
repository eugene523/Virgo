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

    uint c_a = VM::GetConstantId_Str("a");
    uint c_b = VM::GetConstantId_Str("b");
    uint c_c = VM::GetConstantId_Str("c");

    uint c_1 = VM::GetConstantId_Int(1);
    uint c_2 = VM::GetConstantId_Real(3.14);

    ByteCode bc;
    bc.Write_OpCode(OpCode::NewFrame);

    // a = 1
    bc.Write_OpCode(OpCode::PushConstant);
    bc.Write_uint64(c_a);
    bc.Write_OpCode(OpCode::PushConstant);
    bc.Write_uint64(c_1);
    bc.Write_OpCode(OpCode::SetValueByName);

    // b = 1
    bc.Write_OpCode(OpCode::PushConstant);
    bc.Write_uint64(c_b);
    bc.Write_OpCode(OpCode::PushConstant);
    bc.Write_uint64(c_2);
    bc.Write_OpCode(OpCode::SetValueByName);

    // c = a + b
    bc.Write_OpCode(OpCode::PushConstant);
    bc.Write_uint64(c_c);
    bc.Write_OpCode(OpCode::PushConstant);
    bc.Write_uint64(c_a);
    bc.Write_OpCode(OpCode::GetValueByName);
    bc.Write_OpCode(OpCode::PushConstant);
    bc.Write_uint64(c_b);
    bc.Write_OpCode(OpCode::GetValueByName);
    bc.Write_OpCode(OpCode::Add);
    bc.Write_OpCode(OpCode::SetValueByName);

    VM::Execute(bc);
    VM::PrintFrames();

    bc.Write_OpCode(OpCode::CloseFrame);


    return 0;
}