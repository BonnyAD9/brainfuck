#include "err.h" // GENERIC_ERR, IS_ERR

#include <stdio.h> // fprintf, stderr

static int err_code;
static const char *err_msg;

void clear_err(void) {
    err_code = 0;
    err_msg = "No error.";
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
    return err_code;
}

const char *get_err_msg(void) {
    return err_msg;
}

int print_err(void) {
    if (IS_ERR) {
        fprintf(stderr, "%s (code %d)", err_msg, err_code);
    }
    return err_code;
}
