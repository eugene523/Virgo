#ifndef VIRGO_BYTECODE_H
#define VIRGO_BYTECODE_H

#include <cstdint>
#include <cstddef>
#include "Common.h"

enum OpCode : uint64_t
{
    NewFrame,
    CloseFrame,

    LoadConstant,
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

    Add,            //             Obj*, Obj*
    Sub,            //             Obj*, Obj*
    Mul,            //             Obj*, Obj*
    Div,            //             Obj*, Obj*
    Pow,            //             Obj*, Obj*
    Jump,           // pos

    If
    // A : posFalse
    // E : Bool*
};

struct loc {
    uint line;
    uint bcPos;
};

struct ByteCode {
    std::byte * bc;
    uint bcMaxSize;
    uint bcPos = 0;

    // Lines of code stream.
    loc * locs;
    uint  currentLine = 0;
    uint  locsMaxSize;
    uint  locsPos = 0;

    explicit ByteCode();
    ~ByteCode();

    void Enlarge();
    void Print();
    void Write_OpCode(OpCode opCode);
    void Write_uint64(uint64_t i);
    void Write_NewFrame();
    void Write_CloseFrame();
    void Write_LoadConstant(uint64_t id);
    void Write_GetLocalVariable(uint64_t id);
    void Write_SetLocalVariable(uint64_t id);
    void Write_Jump(uint64_t jumpPos);
    void Write_If(uint64_t falsePos);

    void EnlargeLocs();
    void Write_Line(uint line);
};

#endif //VIRGO_BYTECODE_H
