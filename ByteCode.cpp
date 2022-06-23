#include <cstdlib>
#include <iostream>
#include "ByteCode.h"
#include "VM.h"

ByteCode::ByteCode() {
    bcMaxSize = 64;
    bc = (std::byte*)calloc(bcMaxSize, sizeof(std::byte));

    locsMaxSize = 8;
    locs = (loc*)calloc(locsMaxSize, sizeof(loc));
}

ByteCode::~ByteCode() {
    free(bc);
}

void ByteCode::Enlarge() {
    uint newMaxSize = bcMaxSize * 2;
    bc = (std::byte*)realloc(bc, newMaxSize);
    if (bc == nullptr) {
        std::cerr << "Error. Not enough memory.";
    }
    bcMaxSize = newMaxSize;
}

void ByteCode::Print() {
    uint currPos = 0;
    uint nextLocPos = 0;
    for (;;)
    {
        if (locs[nextLocPos].bcPos == currPos) {
            std::cout << "\n\nLine " << locs[nextLocPos].line;
            nextLocPos++;
        }

        OpCode opCode = *((OpCode*)(bc + currPos));
        currPos += sizeof(OpCode);
        switch (opCode)
        {
            case OpCode::NewFrame:
                std::cout << "\nNewFrame";
                break;

            case OpCode::CloseFrame:
                std::cout << "\nCloseFrame";
                break;

            case OpCode::LoadConstant:
            {
                uint64_t id = *((uint64_t*)(bc + currPos));
                currPos += sizeof(uint64_t);
                std::cout << "\nLoadConstant " << VM::ConstantToStr(id);
                break;
            }

            case OpCode::GetLocalVariable:
            {
                uint64_t id = *((uint64_t*)(bc + currPos));
                currPos += sizeof(uint64_t);
                std::cout << "\nGetLocalVariable " << VM::ConstantToStr(id);
                break;
            }

            case OpCode::SetLocalVariable:
            {
                uint64_t id = *((uint64_t*)(bc + currPos));
                currPos += sizeof(uint64_t);
                std::cout << "\nSetLocalVariable " << VM::ConstantToStr(id);
                break;
            }

            case OpCode::Add:
                std::cout << "\nAdd";
                break;

            case OpCode::Sub:
                std::cout << "\nSub";
                break;

            case OpCode::Mul:
                std::cout << "\nMul";
                break;

            case OpCode::Div:
                std::cout << "\nDiv";
                break;

            case OpCode::Pow:
                std::cout << "\nPow";
                break;

            default:
                std::cout << "\nUnknown operation";
                break;
        }
        if (currPos >= bcPos)
            break;
    }
}

void ByteCode::Write_OpCode(OpCode opCode) {
    if ((bcMaxSize - bcPos) < sizeof(OpCode))
        Enlarge();
    *((OpCode*)(bc + bcPos)) = opCode;
    bcPos += sizeof(OpCode);
}

void ByteCode::Write_uint64(uint64_t i) {
    if ((bcMaxSize - bcPos) < sizeof(uint64_t))
        Enlarge();
    *((uint64_t*)(bc + bcPos)) = i;
    bcPos += sizeof(uint64_t);
}

void ByteCode::Write_NewFrame() {
    Write_OpCode(OpCode::NewFrame);
}

void ByteCode::Write_CloseFrame() {
    Write_OpCode(OpCode::CloseFrame);
}

void ByteCode::Write_LoadConstant(uint64_t id) {
    Write_OpCode(OpCode::LoadConstant);
    Write_uint64(id);
}

void ByteCode::Write_GetLocalVariable(uint64_t id) {
    Write_OpCode(OpCode::GetLocalVariable);
    Write_uint64(id);
}

void ByteCode::Write_SetLocalVariable(uint64_t id) {
    Write_OpCode(OpCode::SetLocalVariable);
    Write_uint64(id);
}

void ByteCode::Write_Jump(uint64_t jumpPos) {
    Write_OpCode(OpCode::Jump);
    Write_uint64(jumpPos);
}

void ByteCode::Write_If(uint64_t falsePos) {
    Write_OpCode(OpCode::If);
    Write_uint64(falsePos);
}

void ByteCode::EnlargeLocs() {
    uint newMaxSize = locsMaxSize * 2;
    locs = (loc*)realloc(locs, newMaxSize * sizeof(loc));
    if (locs == nullptr) {
        std::cerr << "Error. Not enough memory.";
    }
    locsMaxSize = newMaxSize;
}

void ByteCode::Write_Line(uint line) {
    if (currentLine == 0) {
        locs[0].line = line;
        locs[0].bcPos = 0;
        locsPos = 1;
        currentLine = line;
        return;
    }

    if (line == currentLine)
        return;

    if (locsPos == locsMaxSize)
        EnlargeLocs();

    locs[locsPos].line = line;
    locs[locsPos].bcPos = bcPos;
    locsPos++;
    currentLine = line;
}
