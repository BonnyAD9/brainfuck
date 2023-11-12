#include <stdio.h>  // FILE, fopen, fclose
#include <stdlib.h> // EXIT_FAILURE

#include "instruction.h" // read_instructions, Instruction
#include "interpreter.h" // interpret
#include "vec.h"         // Vec, VEC_NEW, vec_free, VEC_EXTEND_EXACT
#include "err.h"         // EPRINTF, IS_ERR

int main(int argc, char **argv) {
    if (argc != 2) {
        EPRINTF("Invalid number of arguments.");
        return EXIT_FAILURE;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f) {
        EPRINTF("Cannot open file '%s'", argv[1]);
        return EXIT_FAILURE;
    }

    Vec code = VEC_NEW(Instruction);
    InstructionStream is = inst_s_new(f);

    read_instructions(&is, &code);
    if (IS_ERR) {
        fclose(f);
        vec_free(&code);
        return print_err();
    }

    fclose(f);
    f = NULL;

    Vec tape = VEC_NEW(char);
    VEC_EXTEND_EXACT(char, &tape, 30000, 0);

    interpret(code, &tape);

    vec_free(&code);
    vec_free(&tape);
}
