#include <stdio.h>  // FILE, fopen, fclose
#include <stdlib.h> // EXIT_FAILURE

#include "instruction.h"   // read_instructions, Instruction, inst_s_file
#include "interpreter.h"   // interpret
#include "vec.h"           // Vec, VEC_NEW, vec_free, VEC_EXTEND_EXACT
#include "err.h"           // EPRINTF, IS_ERR
#include "optimizer.h"     // o_acc_stream
#include "ansi-terminal.h" // FG_RED, RESET
#include "arg-parser.h"    // Args, arg_parse, args_print

void print_info(FILE *out, Args *args, Vec *tape);

int main(int argc, char **argv) {
    Args args = arg_parse(argv);

    FILE *f = fopen(args.file, "r");
    if (!f) {
        if (args.print_info) {
            print_info(stdout, &args, NULL);
        }
        return print_err("Cannot open file '%s'", args.file);
    }

    InstructionStream is = o_acc_stream(inst_s_file(f, true));
    if (IS_ERR) {
        if (args.print_info) {
            print_info(stdout, &args, NULL);
        }
        return print_err(NULL);
    }

    Vec code = VEC_NEW(Instruction);
    read_instructions(is, &code);
    if (IS_ERR) {
        if (args.print_info) {
            print_info(stdout, &args, &code);
        }
        vec_free(&code);
        return print_err(NULL);
    }

    if (args.print_info) {
        print_info(stdout, &args, &code);
    }

    Vec tape = VEC_NEW(char);
    VEC_EXTEND_EXACT(char, &tape, args.tape_size, 0);

    interpret(code, &tape);

    vec_free(&code);
    vec_free(&tape);
}

void print_info(FILE *out, Args *args, Vec *code) {
    args_print(args, out);
    err_print_info(out);
    fprintf(
        out,
        "instruction count: %zu\n",
        code ? code->len : 0
    );
}
