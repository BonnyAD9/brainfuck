#include "instruction.h" // Instruction, InstFlags::*

#include <stdio.h>   // FILE, EOF
#include <stdbool.h> // bool, false

#include "vec.h" // Vec, VEC_NEW, VEC_PUSH
#include "err.h" // set_err_msg, GENERIC_ERR, EPROP

Instruction instruction_new(long move, long add, InstFlags flags, long jump) {
    return (Instruction) {
        .move = move,
        .add = add,
        .flags = flags,
        .jump = jump,
    };
}

bool read_instructions(FILE *f, Vec *out) {
    Vec jumps = VEC_NEW(long);

    int cur;
    while ((cur = fgetc(f)) != EOF) {
        switch (cur) {
        case '>':
            VEC_PUSH(Instruction, out, instruction_new(1, 0, INST_NONE, 0));
            EPROP(on_error);
            break;
        case '<':
            VEC_PUSH(Instruction, out, instruction_new(-1, 0, INST_NONE, 0));
            EPROP(on_error);
            break;
        case '+':
            VEC_PUSH(Instruction, out, instruction_new(0, 1, INST_NONE, 0));
            EPROP(on_error);
            break;
        case '-':
            VEC_PUSH(Instruction, out, instruction_new(0, -1, INST_NONE, 0));
            EPROP(on_error);
            break;
        case '.':
            VEC_PUSH(Instruction, out, instruction_new(0, 0, INST_PRINT, 0));
            EPROP(on_error);
            break;
        case ',':
            VEC_PUSH(Instruction, out, instruction_new(0, 0, INST_READ, 0));
            EPROP(on_error);
            break;
        case '[':
            VEC_PUSH(Instruction, out, instruction_new(0, 0, INST_NONE, 0));
            EPROP(on_error);
            VEC_PUSH(long, &jumps, out->len - 1);
            EPROP(on_error);
            break;
        case ']': {
                if (jumps.len == 0) {
                    set_err_msg(GENERIC_ERR, "No matching '[' for ']'");
                    goto on_error;
                }

                long jump = VEC_POP(long, &jumps);
                VEC_AT(Instruction, *out, jump).jump = out->len - jump;

                VEC_PUSH(
                    Instruction,
                    out,
                    instruction_new(0, 0, INST_NONE, jump - out->len)
                );
                EPROP(on_error);
                break;
            }
        }
    }

    if (jumps.len != 0) {
        set_err_msg(GENERIC_ERR, "No matching ']' for '['");
        goto on_error;
    }

    vec_free(&jumps);
    return true;

on_error:
    vec_free(&jumps);
    return false;
}
