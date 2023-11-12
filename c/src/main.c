#include <stdio.h>  // FILE, fopen, fclose
#include <stdlib.h> // EXIT_FAILURE

#include "instruction.h" // read_instructions, Instruction, inst_s_file
#include "interpreter.h" // interpret
#include "vec.h"         // Vec, VEC_NEW, vec_free, VEC_EXTEND_EXACT
#include "err.h"         // EPRINTF, IS_ERR
#include "optimizer.h"   // o_acc_stream

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
    InstructionStream is = o_acc_stream(inst_s_file(f, true));
    if (IS_ERR) {
        return print_err();
    }

    read_instructions(is, &code);
    if (IS_ERR) {
        vec_free(&code);
        return print_err();
    }

    Vec tape = VEC_NEW(char);
    VEC_EXTEND_EXACT(char, &tape, 30000, 0);

    interpret(code, &tape);

    vec_free(&code);
    vec_free(&tape);
}
