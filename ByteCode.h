#ifndef VIRGO_BYTECODE_H
#define VIRGO_BYTECODE_H

#include <cstdint>
#include <cstddef>
#include <vector>
#include "Common.h"

using OpCode_t = uint32_t;
using OpArg    = uint32_t;

enum OpCode : OpCode_t
{
    NoOperation,
    NewContext,
    CloseContext,

    PushConstant,
    // Loads constant taken from the global pool of constants on the stack.
    // Arguments : id (global identifier of a constant)
    // Stack     : ---
    // Result    : Obj* (constant object on the stack)


    GetLocalVariable,
    // Loads local variable on the stack.
    // Arguments : id (global identifier of a string constant used as a name)
    // Stack     : ---
    // Result    : Obj* (local variable)


    SetLocalVariable,
    // Sets value to the local variable.
    // Arguments : id (global identifier of a string constant used as a name)
    // Stack     : Obj* (new value)
    // Result    : ---

    Equal,
    NotEqual,
    Negate,
    Add,
    Subtract,
    Multiply,
    Divide,
    Power,
    Greater,
    GreaterOrEqual,
    Less,
    LessOrEqual,
    Not,
    And,
    Or,
    Jump,
    JumpIfFalse,
    Assert,
    PushInt32,
    SaveByteCodePosition,
    ReadByteCodePosition,
};

struct ByteCode {
    std::byte * bcStream;
    uint maxSize;
    uint pos = 0;

    // Lines of code stream.
    uint numOfLines = 1;
    std::vector<uint> linePos { 0, 0 };

    explicit ByteCode();
    ~ByteCode();

    void Enlarge();

    /*
    void Write_OpCode(OpCode opCode);
    void Write_OpArg(OpArg opArg);
    void Write_int32(int32_t val);
    */

    template<class T>
    void Write(T val) {
        if ((maxSize - pos) < sizeof(T))
            Enlarge();
        *((T*)(bcStream + pos)) = val;
        pos += sizeof(T);
    }

    uint Reserve_OpCode_OpArg();
    void Write_OpCode_OpArg_AtPos(uint atPos, OpCode opCode, OpArg opArg);
    void Write_NewContext();
    void Write_CloseContext();
    void Write_PushConstant(OpArg id);
    void Write_GetLocalVariable(OpArg id);
    void Write_SetLocalVariable(OpArg id);
    void Write_PushInt32(int32_t val);
    void Write_Jump(OpArg toPos);
    void Write_JumpIfFalse(OpArg toPos);
    void Write_Line(uint line);

    void Print();
};

struct ByteCodeReader {
    const std::byte * bcStream;
    uint pos{};
    const uint endPos;

    inline ByteCodeReader(const ByteCode & byteCode) :
    bcStream{byteCode.bcStream}, endPos{byteCode.pos} {}

    inline OpCode Read_OpCode() {
        OpCode opCode = *((OpCode*)(bcStream + pos));
        pos += sizeof(OpCode);
        return opCode;
    }

    inline OpArg Read_OpArg() {
        OpArg opArg = *((OpArg*)(bcStream + pos));
        pos += sizeof(OpArg);
        return opArg;
    }

    inline int32_t Read_int32() {
        int32_t i = *((int32_t*)(bcStream + pos));
        pos += sizeof(int32_t);
        return i;
    }

    inline void Read_OpArg_SetAsPos() {
        pos = *((OpArg*)(bcStream + pos));
    }

    inline void Skip_OpArg() {
        pos += sizeof(OpArg);
    }

    inline bool IsAtEnd() {
        return pos >= endPos;
    }
};

#endif //VIRGO_BYTECODE_H
