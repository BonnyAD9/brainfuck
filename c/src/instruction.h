#ifndef INSTRUCTION_H_INCLUDED
#define INSTRUCTION_H_INCLUDED

#include <stddef.h>  // size_t
#include <stdio.h>   // FILE
#include <stdbool.h> // bool

#include "vec.h" // Vec

typedef enum {
    INST_NONE  = 0x0,
    INST_PRINT = 0x1,
    INST_READ  = 0x2,
} InstFlags;

typedef struct {
    long move; // how much to move the tape position
    long add;  // how much to add to the tape at current index
    // print
    // load
    InstFlags flags;
    long jump; // relative jump offset
    // halt
} Instruction;

Instruction instruction_new(long move, long add, InstFlags flags, long jump);

/// Loads instructions from file `f` into vector `out`
bool read_instructions(FILE *f, Vec *out);

#endif // INSTRUCTION_H_INCLUDED
