#include "interpreter.h"

#include "vec.h"         // Vec, VEC_AT
#include "instruction.h" // Instruction, InstFlags::*
#include "err.h"         // IS_ERR

static inline void _itpt_inst(Interpreter *itpt);

Interpreter itpt_init(Vec code, Args *args) {
    Vec tape = VEC_NEW(unsigned char);
    VEC_EXTEND_EXACT(unsigned char, &tape, args->tape_size, 0);
    if (IS_ERR) {
        return (Interpreter) { 0 };
    }
    return (Interpreter) {
        .code = code,
        .code_index = 0,
        .tape = tape,
        .tape_index = 0,
    };
}

void itpt_free(Interpreter *itpt) {
    vec_free(&itpt->tape);
    itpt->tape_index = 0;
    itpt->code_index = 0;
}

void itpt_all(Interpreter *itpt) {
    // Tape index
    size_t ti = itpt->tape_index;
    // Code index
    size_t ci = itpt->code_index;

    const size_t clen = itpt->code.len;
    const size_t tlen = itpt->tape.len;
    Instruction *restrict const code = (Instruction *)itpt->code.data;
    unsigned char *restrict const tape = (unsigned char *)itpt->tape.data;

    while (ci < clen) {
        Instruction i = code[ci];

        ti += i.move;
        // having theese cycles is actually about 11 times faster than using %
        while (ti >= tlen) {
            ti -= tlen;
        }
        while (ti < 0) {
            ti += tlen;
        }

        tape[ti] += i.add;

        if (i.flags & INST_PRINT) {
            putchar(tape[ti]);
        }

        if (i.flags & INST_READ) {
            tape[ti] = getchar();
        }

        ++ci;
        if (i.jump > 0 && tape[ti] == 0) {
            ci += i.jump;
        } else if (i.jump < 0 && tape[ti] != 0) {
            ci += i.jump;
        }
    }

    itpt->code_index = ci;
    itpt->tape_index = ti;
}

void itpt_inst(Interpreter *itpt) {
    if (itpt->code_index < itpt->code.len) {
        _itpt_inst(itpt);
    }
}

static inline void _itpt_inst(Interpreter *itpt) {
    Instruction i = VEC_AT(Instruction, itpt->code, itpt->code_index);

    itpt->tape_index += i.move;
    // having theese cycles is actually about 11 times faster than using %
    if (itpt->tape_index >= itpt->tape.len) {
        do {
            itpt->tape_index -= itpt->tape.len;
        } while (itpt->tape_index >= itpt->tape.len);
    } else {
        while (itpt->tape_index < 0) {
            itpt->tape_index += itpt->tape.len;
        }
    }

    VEC_AT(unsigned char, itpt->tape, itpt->tape_index) += i.add;

    if (i.flags & INST_PRINT) {
        putchar(VEC_AT(unsigned char, itpt->tape, itpt->tape_index));
    }

    if (i.flags & INST_READ) {
        VEC_AT(unsigned char, itpt->tape, itpt->tape_index) = getchar();
    }

    ++itpt->code_index;
    if (i.jump > 0 && VEC_AT(unsigned char, itpt->tape, itpt->tape_index) == 0) {
        itpt->code_index += i.jump;
    } else if (i.jump < 0 && VEC_AT(unsigned char, itpt->tape, itpt->tape_index) != 0) {
        itpt->code_index += i.jump;
    }
}
