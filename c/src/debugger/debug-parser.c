#include "debug-parser.h"

#include <string.h> // strcmp
#include <ctype.h>  // isspace

#include "../tools/err.h"     // print_err
#include "../tools/parsers.h" // parse_size
#include "debugger.h"         // Debugger

static DbgStepCmd parse_step(char **args);

static char *next_arg(char **args);

DbgCmd dbg_parse_cmd(Debugger *dbg) {
    DbgCmd res = { .action = DA_NONE, };

    char *args = dbg->prompt.data;

    char *cmd = next_arg(&args);
    if (!cmd) {
        return res;
    }

    if (*cmd == ':') {
        res.action = DA_SYSTEM;
        res.system.command = cmd + 1;
        return res;
    }

    if (strcmp(cmd, "?") == 0 || strcmp(cmd, "h") == 0
        || strcmp(cmd, "help") == 0
    ) {
        res.action = DA_HELP;
        if (next_arg(&args)) {
            set_err_msg(
                INVALID_ARGS,
                "Command 'help' doesn't take any arguments."
            );
        }
        return res;
    }
    if (strcmp(cmd, "q") == 0 || strcmp(cmd, "quit") == 0
        || strcmp(cmd, "exit") == 0
    ) {
        res.action = DA_QUIT;
        if (next_arg(&args)) {
            set_err_msg(
                INVALID_ARGS,
                "Command 'help' doesn't take any arguments."
            );
        }
        return res;
    }
    if (strcmp(cmd, "l") == 0 || strcmp(cmd, "list") == 0) {
        res.action = DA_LIST;
        if (next_arg(&args)) {
            set_err_msg(
                INVALID_ARGS,
                "Command 'help' doesn't take any arguments."
            );
        }
        return res;
    }
    if (strcmp(cmd, "n") == 0 || strcmp(cmd, "next") == 0
        || strcmp(cmd, "step") == 0
    ) {
        res.action = DA_STEP;
        res.step = parse_step(&args);
        return res;
    }

    // parse all the variations of the clear command

    if (*cmd != 'c') {
        print_err("Unknown command '%s'", cmd);
        return res;
    }

    ++cmd;

    char cstr[] = "lears";
    int ccnt[sizeof(cstr)] = { 0 };
    for (char *s = cmd; *s; ++s) {
        char *ci = strchr(cstr, *s);
        if (!ci) {
            --cmd;
            print_err("Unknown command '%s'", cmd);
            return res;
        }
        int idx = ci - cstr;
        if (ccnt[idx]) {
            --cmd;
            print_err("Unknown command '%s'", cmd);
            return res;
        }
        ccnt[idx] = 1;
    }

    res.action = DA_CLEAR_SCREEN;
    if (next_arg(&args)) {
        set_err_msg(
            INVALID_ARGS,
            "Command 'clear' doesn't take any arguments."
        );
    }
    return res;
}

static DbgStepCmd parse_step(char **args) {
    DbgStepCmd res = {
        .count = 1,
    };

    char *arg = next_arg(args);
    if (!arg) {
        return res;
    }

    res.count = parse_size(arg);
    if (IS_ERR) {
        return res;
    }

    if (next_arg(args)) {
        set_err_msg(INVALID_ARGS, "Command 'step' takes at most 1 argument.");
    }

    return res;
}

static char *next_arg(char **args) {
    while (**args && isspace(**args)) {
        ++*args;
    }

    if (!**args) {
        return NULL;
    }

    char *res = *args;
    while (**args && !isspace(**args)) {
        ++*args;
    }

    if (**args) {
        **args = 0;
        ++*args;
    }

    return res;
}
