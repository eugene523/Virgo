#include <cstdlib>
#include <iostream>
#include "ByteCode.h"
#include "VM.h"

ByteCode::ByteCode() {
    maxSize = 64;
    stream = (std::byte*)calloc(maxSize, sizeof(std::byte));
}

ByteCode::~ByteCode() {
    free(stream);
}

void ByteCode::Enlarge() {
    uint newMaxSize = maxSize * 2;
    stream = (std::byte*)realloc(stream, newMaxSize);
    if (stream == nullptr) {
        std::cerr << "Error. Not enough memory.";
    }
    maxSize = newMaxSize;
}

void ByteCode::Print() {
    uint currPos = 0;
    for (;;)
    {
        OpCode opCode = *((OpCode*)(stream + currPos));
        currPos += sizeof(OpCode);
        switch (opCode)
        {
            case OpCode::NewFrame :
                std::cout << "\nNewFrame";
                break;

            case OpCode::CloseFrame :
                std::cout << "\nCloseFrame";
                break;

            case OpCode::LoadConstant :
            {
                uint64_t id = *((uint64_t*)(stream + currPos));
                currPos += sizeof(uint64_t);
                std::cout << "\nLoadConstant " << VM::ConstantToStr(id);
                break;
            }

            case OpCode::GetLocalVariable :
            {
                uint64_t id = *((uint64_t*)(stream + currPos));
                currPos += sizeof(uint64_t);
                std::cout << "\nGetLocalVariable " << VM::ConstantToStr(id);
                break;
            }

            case OpCode::SetLocalVariable :
            {
                uint64_t id = *((uint64_t*)(stream + currPos));
                currPos += sizeof(uint64_t);
                std::cout << "\nSetLocalVariable " << VM::ConstantToStr(id);
                break;
            }

            case OpCode::Add :
                std::cout << "\nAdd";
                break;

            case OpCode::Sub :
                std::cout << "\nSub";
                break;

            case OpCode::Mul :
                std::cout << "\nMul";
                break;

            case OpCode::Div :
                std::cout << "\nDiv";
                break;

            case OpCode::Pow :
                std::cout << "\nPow";
                break;

            default :
                std::cout << "\nUnknown operation";
                break;
        }
        if (currPos >= pos)
            break;
    }
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

void ByteCode::Write_NewFrame() {
    Write_OpCode(OpCode::NewFrame);
}

void ByteCode::Write_CloseFrame() {
    Write_OpCode(OpCode::CloseFrame);
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
