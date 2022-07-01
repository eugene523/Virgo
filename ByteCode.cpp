#include <cassert>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include "ByteCode.h"
#include "VM.h"

ByteCode::ByteCode() {
    maxSize = 64;
    bcStream = (std::byte*)calloc(maxSize, sizeof(std::byte));
}

ByteCode::~ByteCode() {
    free(bcStream);
}

void ByteCode::Enlarge() {
    uint newMaxSize = maxSize * 2;
    bcStream = (std::byte*)realloc(bcStream, newMaxSize);
    if (bcStream == nullptr) {
        std::cerr << "Error. Not enough memory.";
        abort();
    }
    maxSize = newMaxSize;
}

void ByteCode::Write_OpCode(OpCode opCode) {
    if ((maxSize - pos) < sizeof(OpCode))
        Enlarge();
    *((OpCode*)(bcStream + pos)) = opCode;
    pos += sizeof(OpCode);
}

void ByteCode::Write_OpArg(OpArg opArg) {
    if ((maxSize - pos) < sizeof(OpArg))
        Enlarge();
    *((OpArg*)(bcStream + pos)) = opArg;
    pos += sizeof(OpArg);
}

uint ByteCode::Reserve_OpCode_OpArg() {
    uint savedPos = pos;
    uint numOfReservedBytes = sizeof(OpCode) + sizeof(OpArg);
    if ((maxSize - pos) < numOfReservedBytes)
        Enlarge();
    pos += numOfReservedBytes;
    return savedPos;
}

void ByteCode::Write_OpCode_OpArg_AtPos(uint atPos, OpCode opCode, OpArg opArg) {
    assert(atPos + sizeof(opCode) + sizeof(OpArg) <= maxSize);
    *((OpCode*)(bcStream + atPos)) = opCode;
    *((OpArg*)(bcStream + atPos + sizeof(OpCode))) = opArg;
}

void ByteCode::Write_NewFrame() {
    Write_OpCode(OpCode::NewFrame);
}

void ByteCode::Write_CloseFrame() {
    Write_OpCode(OpCode::CloseFrame);
}

void ByteCode::Write_LoadConstant(OpArg id) {
    Write_OpCode(OpCode::LoadConstant);
    Write_OpArg(id);
}

void ByteCode::Write_GetLocalVariable(OpArg id) {
    Write_OpCode(OpCode::GetLocalVariable);
    Write_OpArg(id);
}

void ByteCode::Write_SetLocalVariable(OpArg id) {
    Write_OpCode(OpCode::SetLocalVariable);
    Write_OpArg(id);
}

void ByteCode::Write_Jump(OpArg toPos) {
    Write_OpCode(OpCode::Jump);
    Write_OpArg(toPos);
}

void ByteCode::Write_JumpFalse(OpArg toPos) {
    Write_OpCode(OpCode::JumpFalse);
    Write_OpArg(toPos);
}

void ByteCode::Write_Line(uint line) {
    if (line == numOfLines)
        return;

    linePos.push_back(pos);
    numOfLines = line;
}

std::map<OpCode, std::string> OpCodeNames =
{
    { OpCode::Noop,             "Noop"             },
    { OpCode::NewFrame,         "NewFrame"         },
    { OpCode::CloseFrame,       "CloseFrame"       },
    { OpCode::LoadConstant,     "LoadConstant"     },
    { OpCode::GetLocalVariable, "GetLocalVariable" },
    { OpCode::SetLocalVariable, "SetLocalVariable" },
    { OpCode::Eq,               "Eq"               },
    { OpCode::NotEq,            "NotEq"            },
    { OpCode::Neg,              "Neg"              },
    { OpCode::Add,              "Add"              },
    { OpCode::Sub,              "Sub"              },
    { OpCode::Mul,              "Mul"              },
    { OpCode::Div,              "Div"              },
    { OpCode::Pow,              "Pow"              },
    { OpCode::Gr,               "Gr"               },
    { OpCode::GrEq,             "GrEq"             },
    { OpCode::Ls,               "Ls"               },
    { OpCode::LsEq,             "LsEq"             },
    { OpCode::Not,              "Not"              },
    { OpCode::And,              "And"              },
    { OpCode::Or,               "Or"               },
    { OpCode::Jump,             "Jump"             },
    { OpCode::JumpFalse,        "JumpFalse"        },
    { OpCode::Assert,           "Assert"           },
};

void ByteCode::Print() {
    uint currPos = 0;
    uint nextLine = 1;
    for (;;)
    {
        if (nextLine < linePos.size() && linePos[nextLine] == currPos) {
            std::cout << "\n\nLine " << nextLine;
            nextLine++;
        }

        std::cout << '\n' << std::setw(8) << std::left << currPos << " : ";
        OpCode opCode = *((OpCode*)(bcStream + currPos));
        currPos += sizeof(OpCode);
        std::cout << std::setw(20) << std::left;
        switch (opCode)
        {
            case OpCode::Noop :
            case OpCode::NewFrame :
            case OpCode::CloseFrame :
            case OpCode::Eq :
            case OpCode::NotEq :
            case OpCode::Neg :
            case OpCode::Add :
            case OpCode::Sub :
            case OpCode::Mul :
            case OpCode::Div :
            case OpCode::Pow :
            case OpCode::Gr :
            case OpCode::GrEq :
            case OpCode::Ls :
            case OpCode::LsEq :
            case OpCode::Not :
            case OpCode::And :
            case OpCode::Or :
            case OpCode::Assert :
                std::cout << OpCodeNames[opCode];
                break;

            case OpCode::LoadConstant :
            case OpCode::GetLocalVariable :
            case OpCode::SetLocalVariable :
            {
                OpArg id = *((OpArg*)(bcStream + currPos));
                currPos += sizeof(OpArg);
                std::cout << OpCodeNames[opCode] << '\'' << VM::ConstantToStr(id) << '\'';
                break;
            }

            case OpCode::Jump :
            case OpCode::JumpFalse :
            {
                OpArg toPos = *((OpArg*)(bcStream + currPos));
                currPos += sizeof(OpArg);
                std::cout << OpCodeNames[opCode] << "->" << toPos;
                break;
            }

            default:
                std::cout << "Unknown operation";
                break;
        }
        if (currPos >= pos)
            break;
    }
}
