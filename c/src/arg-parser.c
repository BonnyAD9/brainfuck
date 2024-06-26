#include "arg-parser.h"

#include <string.h> // strcmp, size_t
#include <stdlib.h> // strtoull, strtoul

#include "tools/err.h"     // IS_ERR
#include "tools/parsers.h" // parse_size

Args arg_parse(char **argv) {
    Args res = {
        .action = INTERPRET,
        .file = NULL,
        .output = NULL,
        .tape_size = 30000,
        .print_info = 0,
        .print_help = 0,
    };

    while (*++argv) {
        if (strcmp(*argv, "-t") == 0 || strcmp(*argv, "--tape-size") == 0) {
            res.tape_size = parse_size(*++argv);
            if (IS_ERR) {
                return res;
            }
        } else if (strcmp(*argv, "--print-info") == 0) {
            res.print_info = 1;
        } else if (
            strcmp(*argv, "-?") == 0 || strcmp(*argv, "-h") == 0
            || strcmp(*argv, "--help") == 0
        ) {
            res.print_help = 1;
        } else if (
            strcmp(*argv, "-T") == 0 || strcmp(*argv, "--transpile") == 0
        ) {
            res.action = TRANSPILE;
        } else if (
            strcmp(*argv, "-I") == 0 || strcmp(*argv, "--interpret") == 0
        ) {
            res.action = INTERPRET;
        } else if (strcmp(*argv, "-D") == 0 || strcmp(*argv, "--debug") == 0) {
            res.action = DEBUG;
        } else if (
            strcmp(*argv, "-o") == 0 || strcmp(*argv, "--out") == 0
            || strcmp(*argv, "--output") == 0
        ) {
            if (res.output) {
                set_err_msg(
                    INVALID_ARGS,
                    "You can specify only one output file in the arguments."
                );
                return res;
            }
            res.output = *++argv;
            if (!res.output) {
                set_err_msg(
                    INVALID_ARGS,
                    "Missing output file after option '-o'."
                );
                return res;
            }
        } else {
            if (res.file) {
                set_err_msg(
                    INVALID_ARGS,
                    "You can specify only one file in the arguments."
                );
                return res;
            }
            res.file = *argv;
        }
    }

    if (!res.file && !res.print_help) {
        set_err_msg(INVALID_ARGS, "Missing file with program to run.");
    }

    return res;
}

void args_print(Args *args, FILE *out) {
    fprintf(
        out,
        "args: \n"
        "  file: %s\n"
        "  tape size: %zu\n"
        "  print info: %s\n"
        "  print help: %s\n",
        args->file ? args->file : "",
        args->tape_size,
        args->print_info ? "true" : "false",
        args->print_help ? "true" : "false"
    );
}
