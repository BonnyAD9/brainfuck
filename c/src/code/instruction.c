#include "instruction.h" // Instruction, InstFlags::*

#include <stdio.h>   // FILE, EOF
#include <stdbool.h> // bool, false

#include "../tools/vec.h" // Vec, VEC_NEW, VEC_PUSH
#include "../tools/err.h" // set_err_msg, GENERIC_ERR, EPROP

static Instruction inst_s_file_next(FILE *in);
static void noop(void *_) {};

Instruction instruction_new(long move, long add, InstFlags flags, long jump) {
    return (Instruction) {
        .move = move,
        .add = add,
        .flags = flags,
        .jump = jump,
    };
}

InstructionStream inst_s_file(FILE *in, bool free) {
    return (InstructionStream) {
        .data = in,
        .next = (InstructionStreamNextFun)inst_s_file_next,
        .free = free ? (FreeFun)fclose : noop,
    };
}

void inst_print(Instruction inst) {
    if (inst.move > 0) {
        repeat(stdout, '>', inst.move);
    } else if (inst.move < 0) {
        repeat(stdout, '<', -inst.move);
    }

    if (inst.add > 0) {
        repeat(stdout, '+', inst.add);
    } else if (inst.add < 0) {
        repeat(stdout, '-', -inst.add);
    }

    if (inst.flags & INST_PRINT) {
        printf(".");
    }

    if (inst.flags & INST_READ) {
        printf(",");
    }

    if (inst.jump > 0) {
        printf("[");
    } else if (inst.jump < 0) {
        printf("]");
    }
}

bool read_instructions(InstructionStream is, Vec *out) {
    Vec jumps = VEC_NEW(long);

    Instruction cur;
    while (!((cur = is.next(is.data)).flags & INST_HALT)) {
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
    is.free(is.data);
    return true;

on_err:
    vec_free(&jumps);
    is.free(is.data);
    return false;
}

static Instruction inst_s_file_next(FILE *in) {
    int cur;

    while ((cur = fgetc(in)) != EOF) {
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
