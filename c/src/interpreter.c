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

        VEC_AT(char, *tape, ti) =
            (VEC_AT(char, *tape, ti) + i.add) % tape->len;

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
