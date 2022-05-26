#ifndef VIRGO_BYTECODE_H
#define VIRGO_BYTECODE_H

#include <cstdint>
#include <cstddef>
#include "Common.h"

enum OpCode : std::uint64_t
{
/*  Operation       Arguments        Expects on stack   */
    NewFrame,       //
    CloseFrame,     //
    PushConstant,   // id
    GetValueByName, //               Str*
    SetValueByName, //               Str*, Obj*
    Add,            //               Obj*, Obj*
    Sub,            //               Obj*, Obj*
    Mul,            //               Obj*, Obj*
    Div,            //               Obj*, Obj*
    Pow,            //               Obj*, Obj*
    Jump,           // pos

    If
    // A0 : pos_true
    // A1 : pos_false
    // E  : Bool*
};

struct ByteCode {
    std::byte * stream;
    uint maxSize;
    uint pos = 0;

    explicit ByteCode();

    void Enlarge();
    void Write_OpCode(OpCode opCode);
    void Write_uint64(std::uint64_t i);
};

#endif //VIRGO_BYTECODE_H
