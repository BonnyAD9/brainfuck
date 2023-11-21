#include "debugger.h"

#include <stdio.h> // printf, fflush

#include "vec.h" // Vec, VEC_NEW, VEC_EXTEND_EXACT

static void dbg_ui_start(Debugger *dbg);

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

Debugger dbg_start(Debugger *dbg) {
    dbg_ui_start(dbg);
    // TODO: debug cycle
}

static void dbg_ui_start(Debugger *dbg) {
    printf("Press enter to confirm this as the terminal size: ");
    fflush(stdout);
    // TODO request and get terminal size
}
