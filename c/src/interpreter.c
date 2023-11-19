#include "interpreter.h"

#include "vec.h"         // Vec, VEC_AT
#include "instruction.h" // Instruction, InstFlags::*

void interpret(const Vec code, Vec *tape) {
    // Tape index
    size_t ti = 0;
    // Code index
    size_t ci = 0;

    while (ci < code.len) {
        Instruction i = VEC_AT(Instruction, code, ci);

        ti += i.move;
        // having theese cycles is actually about 11 times faster than using %
        while (ti >= tape->len) {
            ti -= tape->len;
        }
        while (ti < 0) {
            ti += tape->len;
        }

        VEC_AT(char, *tape, ti) += i.add;

        if (i.flags & INST_PRINT) {
            putchar(VEC_AT(char, *tape, ti));
        }

        if (i.flags & INST_READ) {
            VEC_AT(char, *tape, ti) = getchar();
        }

        ++ci;
        if (i.jump > 0 && VEC_AT(char, *tape, ti) == 0) {
            ci += i.jump;
        } else if (i.jump < 0 && VEC_AT(char, *tape, ti) != 0) {
            ci += i.jump;
        }
    }
}
