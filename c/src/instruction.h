#ifndef INSTRUCTION_H_INCLUDED
#define INSTRUCTION_H_INCLUDED

#include <stddef.h>  // size_t
#include <stdio.h>   // FILE
#include <stdbool.h> // bool

#include "vec.h" // Vec

typedef enum {
    INST_NONE  = 0x00,
    INST_PRINT = 0x01,
    INST_READ  = 0x02,
    INST_HALT  = 0x04,
    INST_FORW  = 0x08,
    INST_BACK  = 0x10,
    INST_MOVE  = 0x20,
    INST_ADD   = 0x40,

    INST_JUMP = 0x18,
} InstFlags;

typedef struct {
    long move; // how much to move the tape position
    long add;  // how much to add to the tape at current index
    // print
    // load
    InstFlags flags;
    long jump; // relative jump offset
} Instruction;

typedef Instruction (*InstructionStreamNextFun)(void *data);
typedef void (*FreeFun)(void *data);

typedef struct {
    void *data;
    InstructionStreamNextFun next;
    FreeFun free;
} InstructionStream;

Instruction instruction_new(long move, long add, InstFlags flags, long jump);

InstructionStream inst_s_file(FILE *in, bool free);

/// Loads instructions from stream `is` into vector `out`
bool read_instructions(InstructionStream is, Vec *out);

#endif // INSTRUCTION_H_INCLUDED
