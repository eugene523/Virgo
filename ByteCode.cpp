#include <cstdlib>
#include <iostream>
#include "ByteCode.h"

ByteCode::ByteCode() {
    maxSize = 64;
    stream = (std::byte*)calloc(maxSize, sizeof(std::byte));
}

void ByteCode::Enlarge() {
    uint newMaxSize = maxSize * 2;
    stream = (std::byte*)realloc(stream, newMaxSize);
    if (stream == nullptr) {
        std::cerr << "Error. Not enough memory.";
    }
    maxSize = newMaxSize;
}

void ByteCode::Write_OpCode(OpCode opCode) {
    if ((maxSize - pos) < sizeof(OpCode))
        Enlarge();
    *((OpCode*)(stream + pos)) = opCode;
    pos += sizeof(OpCode);
}

void ByteCode::Write_uint64(std::uint64_t i) {
    if ((maxSize - pos) < sizeof(std::uint64_t))
        Enlarge();
    *((std::uint64_t*)(stream + pos)) = i;
    pos += sizeof(std::uint64_t);
}

void ByteCode::Write_LoadConstant(std::uint64_t id) {
    Write_OpCode(OpCode::LoadConstant);
    Write_uint64(id);
}

void ByteCode::Write_GetLocalVariable(std::uint64_t id) {
    Write_OpCode(OpCode::GetLocalVariable);
    Write_uint64(id);
}

void ByteCode::Write_SetLocalVariable(std::uint64_t id) {
    Write_OpCode(OpCode::SetLocalVariable);
    Write_uint64(id);
}

void ByteCode::Write_Jump(std::uint64_t jumpPos) {
    Write_OpCode(OpCode::Jump);
    Write_uint64(jumpPos);
}

void ByteCode::Write_If(std::uint64_t falsePos) {
    Write_OpCode(OpCode::If);
    Write_uint64(falsePos);
}