#include "debugger.h"

#include <stdio.h> // printf, fflush

#include "vec.h"           // Vec, VEC_NEW, VEC_EXTEND_EXACT
#include "ansi-terminal.h" // FG_*
#include "err.h"           // IS_ERR

static void dbg_ui_start(Debugger *dbg);

static void dbg_prompt(Debugger *dbg);

Debugger dbg_init(Args *args, Vec code) {
    Vec tape = VEC_NEW(unsigned char);
    VEC_EXTEND_EXACT(unsigned char, &tape, args->tape_size, 0);
    return (Debugger) {
        .code = code,
        .tape = VEC_NEW(unsigned char),
        .code_index = 0,
        .tape_index = 0,
    };
}

void dbg_free(Debugger *dbg) {
    vec_free(&dbg->prompt);
    vec_free(&dbg->tape);
}

Debugger dbg_start(Debugger *dbg) {
    dbg_ui_start(dbg);
    // TODO: debug cycle
}

static void dbg_ui_start(Debugger *dbg) {
    printf("Use " FG_YELLOW "h " RESET "to show the help.\n");
}

static void dbg_prompt(Debugger *dbg) {
    printf(FG_DARK_GRAY " > " RESET);
    fflush(stdout);
    int c;
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
