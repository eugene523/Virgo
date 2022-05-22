#ifndef VIRGO_BYTECODE_H
#define VIRGO_BYTECODE_H

#include <cstdint>
#include <cstddef>
#include "Common.h"

enum OpCode : std::uint64_t {
    NewFrame,
    CloseFrame,
    PushConstant,   // constant_id
    GetValueByName, // expects Str* on stack
    SetValueByName, // expects Str*, Obj* on stack
    Add,
    Sub,
    Mult,
    Div,
    Pow,

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
