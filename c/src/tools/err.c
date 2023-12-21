#include "err.h" // GENERIC_ERR, IS_ERR

#include <stdio.h>  // fprintf, stderr, perror, vfprintf
#include <errno.h>  // errno
#include <stdarg.h> // va_list, va_start, va_end
#include <stdlib.h> // EXIT_FAILURE

#include "ansi-terminal.h" // FG_RED, RESET

static int err_code;
static const char *err_msg = "No error.";

void clear_err(void) {
    err_code = 0;
    err_msg = "No error.";
    errno = 0;
}

int pop_err(const char **msg) {
    if (msg) {
        *msg = err_msg;
    }
    int err = err_code;
    clear_err();
    return err;
}

void set_err(void) {
    err_code = GENERIC_ERR;
    err_msg = "An error occured.";
}

void set_err_msg(int code, const char *msg) {
    err_code = code;
    err_msg = msg ? msg : "An error occured.";
}

int get_err(void) {
    if (err_code) {
        return err_code;
    }
    return errno;
}

const char *get_err_msg(void) {
    return err_msg;
}

int print_err(const char *str, ...) {
    if (err_code) {
        if (str && *str) {
            fprintf(stderr, FG_RED "error" RESET ": ");

            va_list va;
            va_start(va, str);
            vfprintf(stderr, str, va);
            va_end(va);

            fprintf(stderr, "\n");

            return err_code;
        }
        fprintf(
            stderr,
            FG_RED "error" RESET ": %s\n",
            err_msg
        );
    }

    if (errno) {
        if (str && *str) {
            fprintf(stderr, FG_RED "error" RESET ": ");

            va_list va;
            va_start(va, str);
            vfprintf(stderr, str, va);
            va_end(va);

            fprintf(stderr, ": ");

            perror(NULL);
        } else {
            perror(FG_RED "error" RESET);
        }
        return errno;
    }

    if (str && *str) {
        fprintf(stderr, FG_RED "error" RESET ": ");

        va_list va;
        va_start(va, str);
        vfprintf(stderr, str, va);
        va_end(va);

        fprintf(stderr, "\n");

        return EXIT_FAILURE;
    }

    return NO_ERROR;
}

void err_print_info(FILE *out) {
    fprintf(
        out,
        "errors:\n"
        "  code: %d\n"
        "  msg: %s\n"
        "  errno: %d\n",
        err_code,
        err_msg,
        (int)errno
    );
}
