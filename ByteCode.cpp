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