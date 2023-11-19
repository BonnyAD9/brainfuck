#include "arg-parser.h"

#include <string.h> // strcmp, size_t
#include <stdlib.h> // strtoull, strtoul

#include "err.h" // IS_ERR

static size_t parse_size(char *str);

Args arg_parse(char **argv) {
    Args res = {
        .tape_size = 30000,
        .file = NULL,
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
            || strcmp(*argv, "--help")
        ) {
            res.print_help = 1;
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

static size_t parse_size(char *str) {
    if (!str) {
        set_err_msg(INVALID_ARGS, "Missing unsigned number argument.");
        return 0;
    }

    if (sizeof(size_t) == sizeof(long long)) {
        char *end;
        size_t res = strtoull(str, &end, 10);
        if (IS_ERR) {
            return 0;
        }
        if (*end || end == str) {
            set_err_msg(INVALID_ARGS, "Invalid unsigned number.");
            return 0;
        }
        return res;
    }

    char *end;
    size_t res = strtoul(str, &end, 10);
    if (IS_ERR) {
        return 0;
    }
    if (*end || end == str) {
        set_err_msg(INVALID_ARGS, "Invalid unsigned number");
        return 0;
    }
    return res;
}
