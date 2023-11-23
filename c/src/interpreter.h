#ifndef INTERPRETER_H_INCLUDED
#define INTERPRETER_H_INCLUDED

#include "vec.h"         // Vec
#include "arg-parser.h"  // Args
#include "instruction.h" // Instruction

typedef struct {
    Vec code;
    Vec tape;
    size_t tape_index;
    size_t code_index;
} Interpreter;

Interpreter itpt_init(Vec code, Args *args);

void itpt_free(Interpreter *itpt);

void itpt_all(Interpreter *itpt);

void itpt_inst(Interpreter *itpt);



#endif // INTERPRETER_H_INCLUDED
