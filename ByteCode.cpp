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

void ByteCode::Write_NewContext() {
    Write<OpCode>(OpCode::NewContext);
}

void ByteCode::Write_CloseContext() {
    Write<OpCode>(OpCode::CloseContext);
}

void ByteCode::Write_PushConstant(OpArg id) {
    Write<OpCode>(OpCode::PushConstant);
    Write<OpArg>(id);
}

void ByteCode::Write_GetLocalVariable(OpArg id) {
    Write<OpCode>(OpCode::GetLocalVariable);
    Write<OpArg>(id);
}

void ByteCode::Write_SetLocalVariable(OpArg id) {
    Write<OpCode>(OpCode::SetLocalVariable);
    Write<OpArg>(id);
}

void ByteCode::Write_PushInt32(int32_t val) {
    Write<OpCode>(OpCode::PushInt32);
    Write<int32_t>(val);
}

void ByteCode::Write_Jump(OpArg toPos) {
    Write<OpCode>(OpCode::Jump);
    Write<OpArg>(toPos);
}

void ByteCode::Write_JumpIfFalse(OpArg toPos) {
    Write<OpCode>(OpCode::JumpIfFalse);
    Write<OpArg>(toPos);
}

void ByteCode::Write_Line(uint line) {
    if (line == numOfLines)
        return;

    linePos.push_back(pos);
    numOfLines = line;
}

std::map<OpCode, std::string> OpCodeNames =
{
    { OpCode::NoOperation,      "NoOperation"      },
    { OpCode::NewContext,       "NewContext"       },
    { OpCode::CloseContext,     "CloseContext"     },
    { OpCode::PushConstant,     "PushConstant"     },
    { OpCode::GetLocalVariable, "GetLocalVariable" },
    { OpCode::SetLocalVariable, "SetLocalVariable" },
    { OpCode::Equal,            "Equal"            },
    { OpCode::NotEqual,         "NotEqual"         },
    { OpCode::Negate,           "Negate"           },
    { OpCode::Add,              "Add"              },
    { OpCode::Subtract,         "Subtract"         },
    { OpCode::Multiply,         "Multiply"         },
    { OpCode::Divide,           "Divide"           },
    { OpCode::Power,            "Power"            },
    { OpCode::Greater,          "Greater"          },
    { OpCode::GreaterOrEqual,   "GreaterOrEqual"   },
    { OpCode::Less,             "Less"             },
    { OpCode::LessOrEqual,      "LessOrEqual"      },
    { OpCode::Not,              "Not"              },
    { OpCode::And,              "And"              },
    { OpCode::Or,               "Or"               },
    { OpCode::Jump,             "Jump"             },
    { OpCode::JumpIfFalse,      "JumpIfFalse"      },
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
            case OpCode::NoOperation:
            case OpCode::NewContext:
            case OpCode::CloseContext:
            case OpCode::Equal:
            case OpCode::NotEqual:
            case OpCode::Negate:
            case OpCode::Add:
            case OpCode::Subtract:
            case OpCode::Multiply:
            case OpCode::Divide:
            case OpCode::Power:
            case OpCode::Greater:
            case OpCode::GreaterOrEqual:
            case OpCode::Less:
            case OpCode::LessOrEqual:
            case OpCode::Not:
            case OpCode::And:
            case OpCode::Or:
            case OpCode::Assert :
                std::cout << OpCodeNames[opCode];
                break;

            case OpCode::PushConstant:
            case OpCode::GetLocalVariable:
            case OpCode::SetLocalVariable:
            {
                OpArg id = *((OpArg*)(bcStream + currPos));
                currPos += sizeof(OpArg);
                std::cout << OpCodeNames[opCode] << '\'' << VM::ConstantToStr(id) << '\'';
                break;
            }

            case OpCode::Jump:
            case OpCode::JumpIfFalse:
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
