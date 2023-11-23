#ifndef DEBUGGER_H_INCLUDED
#define DEBUGGER_H_INCLUDED

#include <stddef.h> // size_t

#include "vec.h"        // Vec
#include "arg-parser.h" // Args

typedef struct {
    // Data type: Instruction
    Vec code;
    // Data type: unsigned char
    Vec tape;
    size_t code_index;
    size_t tape_index;
    // Data type: char
    Vec prompt;
    size_t term_width;
} Debugger;

Debugger dbg_init(Args *args, Vec code);

void dbg_free(Debugger *dbg);

void dbg_start(Debugger *dbg);

#endif // DEBUGGER_H_INCLUDED
