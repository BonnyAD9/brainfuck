#include <stdio.h>  // FILE, fopen, fclose
#include <stdlib.h> // EXIT_FAILURE

#include "instruction.h"   // read_instructions, Instruction, inst_s_file
#include "interpreter.h"   // interpret
#include "vec.h"           // Vec, VEC_NEW, vec_free, VEC_EXTEND_EXACT
#include "err.h"           // EPRINTF, IS_ERR
#include "optimizer.h"     // o_acc_stream
#include "ansi-terminal.h" // FG_*, SET_*, RESET
#include "arg-parser.h"    // Args, arg_parse, args_print

void print_info(FILE *out, Args *args, Vec *tape);
void help(void);

int main(int argc, char **argv) {
    Args args = arg_parse(argv);

    if (args.print_help) {
        help();
    }

    if (!args.file || IS_ERR) {
        if (args.print_info) {
            print_info(stdout, &args, NULL);
        }
        return print_err(NULL);
    }

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

void help(void) {
    printf(
        "Welcome in help for " SET_ITALIC FG_GREEN "C brainfuck" RESET
        " by %s.\n"
        FG_GREEN "Version:" RESET " 1.0.0\n"
        "\n"
        FG_GREEN "Usage:\n"
        FG_WHITE "  brainfuck (-? | -h | --help) " FG_DARK_GRAY "[flags]\n"
        "\n"
        FG_WHITE "  brainfuck <file> " FG_DARK_GRAY "[flags]\n"
        "\n"
        FG_GREEN "Flags:\n"
        FG_YELLOW "  -?  -h  --help\n" RESET
        "    Shows this help.\n"
        "\n"
        FG_YELLOW "  -t  --tape-size " FG_WHITE "<size>\n" RESET
        "    Sets the size of the tape (memory for the brainfuck code) in\n"
        "    bytes (cells).\n"
        "\n"
        FG_YELLOW "  --print-info\n" RESET
        "    Print information about the configuration and state.\n"
        "\n",
        FG_RGB(250, 50, 170) "B" FG_RGB(240, 50, 180) "o" FG_RGB(230, 50, 190) "n"
        FG_RGB(220, 50, 200) "n" FG_RGB(210, 50, 210) "y" FG_RGB(200, 50, 220) "A"
        FG_RGB(190, 50, 230) "D" FG_RGB(180, 50, 240) "9" RESET
    );
}
