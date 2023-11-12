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

InstructionStream inst_s_new(FILE *in) {
    return (InstructionStream) {
        .in = in,
    };
}

Instruction inst_s_next(InstructionStream *is) {
    int cur;

    while ((cur = fgetc(is->in)) != EOF) {
        switch (cur) {
        case '>':
            return instruction_new(1, 0, INST_MOVE, 0);
        case '<':
            return instruction_new(-1, 0, INST_MOVE, 0);
        case '+':
            return instruction_new(0, 1, INST_ADD, 0);
        case '-':
            return instruction_new(0, -1, INST_ADD, 0);
        case '.':
            return instruction_new(0, 0, INST_PRINT, 0);
        case ',':
            return instruction_new(0, 0, INST_READ, 0);
        case '[':
            return instruction_new(0, 0, INST_FORW, 0);
        case ']':
            return instruction_new(0, 0, INST_BACK, 0);
        }
    }

    return instruction_new(0, 0, INST_HALT, 0);
}

bool read_instructions(InstructionStream *is, Vec *out) {
    Vec jumps = VEC_NEW(long);

    Instruction cur;
    while (!((cur = inst_s_next(is)).flags & INST_HALT)) {
        if (!(cur.flags & INST_JUMP)) {
            VEC_PUSH(Instruction, out, cur);
            EPROP(on_err);
            continue;
        }

        if (cur.flags & INST_FORW) {
            VEC_PUSH(long, &jumps, out->len);
            EPROP(on_err);
            VEC_PUSH(Instruction, out, cur);
            EPROP(on_err);
            continue;
        }

        if (jumps.len == 0) {
            set_err_msg(GENERIC_ERR, "No matching '[' for ']'");
            goto on_err;
        }

        long jump = VEC_POP(long, &jumps);
        VEC_AT(Instruction, *out, jump).jump = out->len - jump;
        cur.jump = jump - out->len;
        VEC_PUSH(Instruction, out, cur);
        EPROP(on_err);
    }

    if (jumps.len != 0) {
        set_err_msg(GENERIC_ERR, "No matching ']' for '['");
        goto on_err;
    }

    vec_free(&jumps);
    return true;

on_err:
    vec_free(&jumps);
    return false;
}
