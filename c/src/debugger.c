#include "debugger.h"

#include <stdio.h>  // printf, fflush
#include <string.h> // strmcp

#include "vec.h"           // Vec, VEC_NEW, VEC_EXTEND_EXACT
#include "ansi-terminal.h" // FG_*
#include "err.h"           // IS_ERR
#include "instruction.h"   // inst_print, Instruction

enum DbgAct {
    DA_NONE,
    DA_HELP,
    DA_QUIT,
    DA_CLEAR_SCREEN,
    DA_LIST,
    DA_STEP,
};
#define DbgAct enum DbgAct

struct DbgCmd {
    DbgAct action;
};
#define DbgCmd struct DbgCmd

static void dbg_ui_start(Debugger *dbg);
static void dbg_prompt(Debugger *dbg);
static void dbg_clear(Debugger *dbg);
static void dbg_list(Debugger *dbg);
static void dbg_list_code(Debugger *dbg);
static void dbg_list_tape(Debugger *dbg);
static void dbg_step(Debugger *dbg, size_t count);
static DbgCmd dbg_parse_cmd(Debugger *dbg);
static void dbg_help(Debugger *dbg);

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
            break;
        case DA_LIST:
            dbg_list(dbg);
            break;
        case DA_STEP:
            dbg_step(dbg, 1);
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
    if (c == EOF) {
        clearerr(stdin);
        printf("\n");
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
    if (strcmp(str, "l") == 0 || strcmp(str, "list") == 0) {
        res.action = DA_LIST;
        return res;
    }
    if (strcmp(str, "n") == 0 || strcmp(str, "next") == 0
        || strcmp(str, "step") == 0
    ) {
        res.action = DA_STEP;
        return res;
    }

    // parse all the variations of the clear command

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
        FG_YELLOW "  <anything that starts with 'c' and continues with any\n"
        "  letters from the string 'lears' and is not one of the previous\n"
        "  commands (e.g. 'clear', 'cls', 'claer', ...)>\n" RESET
        "    " SET_STRIKETROUGH "Claers" RESET "Clears the entire screen and "
        "buffer.\n"
        "\n"
        FG_YELLOW "  l  list\n" RESET
        "    Prints the code close to the active instruction.\n"
        "\n"
        FG_YELLOW "  q  quit  exit\n" RESET
        "    exits the debugger\n"
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

static void dbg_list(Debugger *dbg) {
    dbg_list_tape(dbg);
    dbg_list_code(dbg);
}

static void dbg_list_code(Debugger *dbg) {
    size_t center = dbg->term_width / 2;
    size_t start = dbg->itpt->code_index <= center
        ? 0
        : dbg->itpt->code_index - center;
    size_t end = start + dbg->term_width <= dbg->itpt->code.len
        ? start + dbg->term_width
        : dbg->itpt->code.len;
    size_t diff = end - start;
    if (diff < dbg->term_width) {
        start = diff > start ? 0 : start - diff;
    }

    size_t d_diff = start % 5;
    size_t d_start = d_diff == 0 ? start : start - d_diff + 5;
    D_MOVE_RIGHT(d_diff);

    for (size_t i = d_start; i < end; i += 5) {
        printf(SAVE_CURSOR "%zu" RESTORE_CURSOR MOVE_RIGHT(5), i);
    }

    printf("\n");

    for (size_t i = start; i < end; ++i) {
        inst_print(VEC_AT(Instruction, dbg->itpt->code, i));
    }

    size_t screen_index = dbg->itpt->code_index - start;

    printf("\n");
    D_MOVE_RIGHT(screen_index);
    printf("^\n");
}

static void dbg_list_tape(Debugger *dbg) {
    size_t i_len = 3;

    size_t count = dbg->term_width / i_len;
    size_t c_center = count / 2;
    size_t c_start = dbg->itpt->tape_index <= c_center
        ? 0
        : dbg->itpt->code_index - c_center;
    size_t c_end = c_start + count <= dbg->itpt->tape.len
        ? c_start + count
        : dbg->itpt->tape.len;
    size_t c_diff = c_end - c_start;
    if (c_diff < count) {
        c_start = c_diff > c_start ? 0 : c_start - c_diff;
    }

    for (size_t i = c_start; i < c_end; ++i) {
        printf("%02x ", (int)VEC_AT(unsigned char, dbg->itpt->tape, i));
    }

    size_t screen_index = (dbg->itpt->tape_index - c_start) * i_len;

    printf("\n");
    D_MOVE_RIGHT(screen_index);
    printf("^\n");
}

static void dbg_step(Debugger *dbg, size_t count) {
    while (count--) {
        itpt_inst(dbg->itpt);
    }
    dbg_list(dbg);
}
