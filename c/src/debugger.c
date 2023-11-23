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
    DA_CLEAR_SCREEN,
};
#define DbgAct enum DbgAct

struct DbgCmd {
    DbgAct action;
};
#define DbgCmd struct DbgCmd

static void dbg_ui_start(Debugger *dbg);
static void dbg_prompt(Debugger *dbg);
static void dbg_clear(Debugger *dbg);
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
        case DA_CLEAR_SCREEN:
            dbg_clear(dbg);
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
    if (strcmp(str, "?") == 0 || strcmp(str, "h") == 0
        || strcmp(str, "help") == 0
    ) {
        res.action = DA_HELP;
        return res;
    }
    if (strcmp(str, "q") == 0 || strcmp(str, "quit") == 0
        || strcmp(str, "exit") == 0
    ) {
        res.action = DA_QUIT;
        return res;
    }

    if (*str != 'c') {
        print_err("Unknown command '%s'", str);
        return res;
    }

    ++str;

    char cstr[] = "lears";
    int ccnt[sizeof(cstr)] = { 0 };
    for (char *s = str; *s; ++s) {
        char *ci = strchr(cstr, *s);
        if (!ci) {
            --str;
            print_err("Unknown command '%s'", str);
            return res;
        }
        int idx = ci - cstr;
        if (ccnt[idx]) {
            --str;
            print_err("Unknown command '%s'", str);
            return res;
        }
        ccnt[idx] = 1;
    }

    res.action = DA_CLEAR_SCREEN;
    return res;
}

static void dbg_help(Debugger *dbg) {
    printf(
        "Welcome to " FG_GREEN SET_ITALIC "C brainfuck debugger " RESET "help "
        "by %s\n"
        "\n"
        FG_GREEN "Usage:\n"
        FG_WHITE "  <command>\n"
        "\n"
        FG_GREEN "Commands:\n"
        FG_YELLOW "  ?  h  help\n" RESET
        "    shows this help\n"
        "\n"
        FG_YELLOW "  q  quit  exit\n" RESET
        "    exits the debugger\n"
        "\n"
        FG_YELLOW "  <anything that starts with 'c' and continues with any\n"
        "  letters from the string 'lears' and is not one of the previous\n"
        "  commands (e.g. 'clear', 'cls', 'claer', ...)>\n" RESET
        "    " SET_STRIKETROUGH "Claers" RESET "Clears the entire screen and "
        "buffer.\n"
        "\n",
        FG_RGB(250, 50, 170) "B" FG_RGB(240, 50, 180) "o"
        FG_RGB(230, 50, 190) "n" FG_RGB(220, 50, 200) "n"
        FG_RGB(210, 50, 210) "y" FG_RGB(200, 50, 220) "A"
        FG_RGB(190, 50, 230) "D" FG_RGB(180, 50, 240) "9" RESET
    );
}

static void dbg_clear(Debugger *dbg) {
    printf(CLEAR);
    fflush(stdin);
}
