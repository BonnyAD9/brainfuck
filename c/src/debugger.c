#include "debugger.h"

#include <stdio.h>  // printf, fflush
#include <string.h> // strmcp
#include <stdlib.h> // system

#include "vec.h"           // Vec, VEC_NEW, VEC_EXTEND_EXACT
#include "ansi-terminal.h" // FG_*
#include "err.h"           // IS_ERR
#include "instruction.h"   // inst_print, Instruction
#include "debug-parser.h"  // DbgCmd, DbgAct::*
#include "debug-ui.h"      // dbg_ui_*

static void dbg_clear(Debugger *dbg);
static void dbg_list(Debugger *dbg);
static void dbg_step(Debugger *dbg, DbgStepCmd args);

Debugger dbg_init(Args *args, Interpreter *itpt) {
    return (Debugger) {
        .itpt = itpt,
        .prompt = VEC_NEW(char),
        .term_width = 79,
    };
}

void dbg_free(Debugger *dbg) {
    vec_free(&dbg->prompt);
}

void dbg_start(Debugger *dbg) {
    dbg_ui_start(dbg);

    while (true) {
        dbg_ui_prompt(dbg);
        if (IS_ERR) {
            return;
        }
        DbgCmd cmd = dbg_parse_cmd(dbg);
        if (IS_ERR) {
            print_err(NULL);
            pop_err(NULL);
            continue;
        }

        switch (cmd.action) {
        case DA_HELP:
            dbg_ui_help(dbg);
            break;
        case DA_CLEAR_SCREEN:
            dbg_clear(dbg);
            break;
        case DA_LIST:
            dbg_list(dbg);
            break;
        case DA_STEP:
            dbg_step(dbg, cmd.step);
            break;
        case DA_SYSTEM:
            system(cmd.system.command);
            break;
        case DA_QUIT:
            break;
        case DA_NONE:
            break;
        }

        if (cmd.action == DA_QUIT) {
            break;
        }
    }
}

static void dbg_clear(Debugger *dbg) {
    printf(CLEAR);
    fflush(stdin);
}

static void dbg_list(Debugger *dbg) {
    dbg_ui_list_tape(dbg);
    dbg_ui_list_code(dbg);
}

static void dbg_step(Debugger *dbg, DbgStepCmd args) {
    while (args.count--) {
        itpt_inst(dbg->itpt);
    }
    dbg_list(dbg);
}
