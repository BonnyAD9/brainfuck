#ifndef DEBUGGER_H_INCLUDED
#define DEBUGGER_H_INCLUDED

#include <stddef.h> // size_t

#include "../tools/vec.h"        // Vec
#include "../arg-parser.h"       // Args
#include "../code/interpreter.h" // Interpreter

typedef struct {
    Interpreter *itpt;
    // Data type: char
    Vec prompt;
    size_t term_width;
} Debugger;

Debugger dbg_init(Args *args, Interpreter *itpt);

void dbg_free(Debugger *dbg);

void dbg_start(Debugger *dbg);

#endif // DEBUGGER_H_INCLUDED
