#include <stdio.h>  // FILE, fopen, fclose
#include <stdlib.h> // EXIT_FAILURE

#include "code/instruction.h"   // read_instructions, Instruction, inst_s_file
#include "code/interpreter.h"   // interpret
#include "code/optimizer.h"     // o_acc_stream
#include "code/c-transpiler.h"  // c_transpile
#include "tools/vec.h"          // Vec, VEC_NEW, vec_free, VEC_EXTEND_EXACT
#include "tools/err.h"          // EPRINTF, IS_ERR
#include "tools/ansi-terminal.h"// FG_*, SET_*, RESET
#include "tools/stream/file_stream.h"
#include "tools/stream/buf_stream.h"
#include "debugger/debugger.h"  // dbg_init, dbg_start
#include "arg-parser.h"         // Args, arg_parse, args_print

void handle_interpret(Vec code, Args *args);
void handle_transpile(const Vec code, Args *args);
void handle_debug(Vec code, Args *args);
void print_info(FILE *out, Args *args, Vec *tape);
void must(Args *args);
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
        must(&args);
        return print_err("Cannot open file '%s'", args.file);
    }

    InstructionStream is = inst_s_file(f, true);
    if (IS_ERR) {
        must(&args);
        return print_err(NULL);
    }

    // enable simple optimizations when not in debug mode
    if (args.action != DEBUG) {
        is = o_acc_stream(is, args.tape_size);
        if (IS_ERR) {
            must(&args);
            return print_err(NULL);
        }
    }

    Vec code = VEC_NEW(Instruction);
    read_instructions(is, &code);
    if (IS_ERR) {
        must(&args);
        vec_free(&code);
        return print_err(NULL);
    }

    if (args.print_info) {
        print_info(stdout, &args, &code);
    }

    switch (args.action) {
    case INTERPRET:
        handle_interpret(code, &args);
        break;
    case TRANSPILE:
        handle_transpile(code, &args);
        break;
    case DEBUG:
        handle_debug(code, &args);
        break;
    }

    vec_free(&code);
    return print_err(NULL);
}

void handle_interpret(const Vec code, Args *args) {
    if (args->output) {
        WPRINTF("Unused argument '-o'");
    }

    Interpreter itpt = itpt_init(code, args, FS_STDIN);
    if (IS_ERR) {
        return;
    }
    itpt_all(&itpt);
    itpt_free(&itpt);
}

void handle_transpile(const Vec code, Args *args) {
    FILE *output = args->output ? fopen(args->output, "w") : stdout;
    if (IS_ERR) {
        print_err("Failed to open file '%s' for writing.", args->output);
    }

    c_transpile(output, code, args->tape_size);
}

void handle_debug(Vec code, Args *args) {
    if (args->output) {
        WPRINTF("Unused argument '-o'");
    }

    BufStream *stream = bs_new(FS_STDIN, '\n');
    if (IS_ERR) {
        print_err(NULL);
        return;
    }

    Interpreter itpt = itpt_init(code, args, bs_to_stream(stream));
    if (IS_ERR) {
        bs_free(&stream);
        print_err(NULL);
        return;
    }
    Debugger dbg = dbg_init(args, &itpt);
    if (IS_ERR) {
        itpt_free(&itpt);
        print_err(NULL);
        return;
    }
    dbg_start(&dbg);
    dbg_free(&dbg);
    itpt_free(&itpt);
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

void must(Args *args) {
    if (args->print_info) {
        print_info(stdout, args, NULL);
    }
}

void help(void) {
    printf(
        "Welcome in help for " SET_ITALIC FG_GREEN "C brainfuck" RESET
        " by %s.\n"
        FG_GREEN "Version:" RESET " 1.1.0\n"
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
        "\n"
        FG_YELLOW "  -T  --transpile\n" RESET
        "    Transpile instead of interpret. See " FG_DARK_GREEN "*1 " RESET
        "and " FG_DARK_YELLOW "-o" RESET ".\n"
        "\n"
        FG_YELLOW "  -I  --interpret\n" RESET
        "    Interpret, this is the default. See " FG_DARK_GREEN "*1.\n"
        "\n"
        FG_YELLOW "  -o  --out  --output " FG_WHITE "<file>\n" RESET
        "    Sets the output file of operation. Stdout by default.\n"
        "\n"
        FG_GREEN "*1\n" RESET
        "  In case that there are multiple flags from the group "
        FG_DARK_YELLOW "-T" RESET ", " FG_DARK_YELLOW "-I" RESET ",\n"
        "  the last flag takes effect.\n"
        "\n",
        FG_RGB(250, 50, 170) "B" FG_RGB(240, 50, 180) "o"
        FG_RGB(230, 50, 190) "n" FG_RGB(220, 50, 200) "n"
        FG_RGB(210, 50, 210) "y" FG_RGB(200, 50, 220) "A"
        FG_RGB(190, 50, 230) "D" FG_RGB(180, 50, 240) "9" RESET
    );
}
