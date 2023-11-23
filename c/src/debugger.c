#include "debugger.h"

#include <stdio.h>  // printf, fflush
#include <string.h> // strmcp

#include "vec.h"           // Vec, VEC_NEW, VEC_EXTEND_EXACT
#include "ansi-terminal.h" // FG_*
#include "err.h"           // IS_ERR

enum DbgAct {
    DA_NONE,
    DA_HELP,
    DA_QUIT,
};
#define DbgAct enum DbgAct

struct DbgCmd {
    DbgAct action;
};
#define DbgCmd struct DbgCmd

static void dbg_ui_start(Debugger *dbg);
static void dbg_prompt(Debugger *dbg);
static DbgCmd dbg_parse_cmd(Debugger *dbg);
static void dbg_help(Debugger *dbg);

Debugger dbg_init(Args *args, Vec code) {
    Vec tape = VEC_NEW(unsigned char);
    VEC_EXTEND_EXACT(unsigned char, &tape, args->tape_size, 0);
    return (Debugger) {
        .code = code,
        .tape = tape,
        .code_index = 0,
        .tape_index = 0,
        .prompt = VEC_NEW(char),
    };
}

void dbg_free(Debugger *dbg) {
    vec_free(&dbg->prompt);
    vec_free(&dbg->tape);
}

void dbg_start(Debugger *dbg) {
    dbg_ui_start(dbg);

    dbg_prompt(dbg);
    if (IS_ERR) {
        print_err(NULL);
        pop_err(NULL);
    }
    DbgCmd cmd;
    while ((cmd = dbg_parse_cmd(dbg)).action != DA_QUIT) {
        switch (cmd.action) {
        case DA_HELP:
            dbg_help(dbg);
            break;
        default:
            break;
        }
        dbg_prompt(dbg);
        if (IS_ERR) {
            print_err(NULL);
            pop_err(NULL);
        }
    }
}

static void dbg_ui_start(Debugger *dbg) {
    printf("Use " FG_YELLOW "h " RESET "to show the help.\n");
}

static void dbg_prompt(Debugger *dbg) {
    printf(FG_MAGENTA " > " RESET);
    fflush(stdout);
    int c;
    vec_clear(&dbg->prompt);
    while ((c = getchar()) != EOF && c != '\n') {
        VEC_PUSH(char, &dbg->prompt, c);
        if (IS_ERR) {
            return;
        }
    }
    VEC_PUSH(char, &dbg->prompt, 0);
    if (IS_ERR) {
        return;
    }
    vec_pop(&dbg->prompt);
}

static DbgCmd dbg_parse_cmd(Debugger *dbg) {
    DbgCmd res = { .action = DA_NONE, };

    char *str = dbg->prompt.data;

    if (*str == 0) {
        return res;
    }
    if (strcmp(str, "h") == 0 || strcmp(str, "help") == 0) {
        res.action = DA_HELP;
        return res;
    }
    if (strcmp(str, "q") == 0 || strcmp(str, "quit") == 0
        || strcmp(str, "exit") == 0
    ) {
        res.action = DA_QUIT;
        return res;
    }
    print_err("Unknown command '%s'", str);
    return res;
}

static void dbg_help(Debugger *dbg) {
    // TODO: implement dbg help
    print_err("Help not implemented");
}
