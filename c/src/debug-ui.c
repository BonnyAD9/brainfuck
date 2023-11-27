#include "debug-ui.h"

#include <stdio.h> // printf, stdin, EOF, stdout

#include "debugger.h"      // Debugger
#include "ansi-terminal.h" // FG_*, RESET
#include "err.h"           // IS_ERR
#include "vec.h"           // VEC_*, vec_pop

#ifndef MAX_EOF_RETRIES
#define MAX_EOF_RETRIES 8
#endif // MAX_EOF_RETRIES

void dbg_ui_start(Debugger *dbg) {
    printf("Use " FG_YELLOW "h " RESET "to show the help.\n");
}

void dbg_ui_prompt(Debugger *dbg) {
    static size_t eof_count = 0;

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

        if (++eof_count > MAX_EOF_RETRIES) {
            set_err_msg(GENERIC_ERR, "Eof limit reached in debugger.");
            return;
        }

        if (eof_count == MAX_EOF_RETRIES) {
            WPRINTF("Sending EOF one more time will exit the debugger.");
        }
    } else {
        eof_count = 0;
    }

    VEC_PUSH(char, &dbg->prompt, 0);
    if (IS_ERR) {
        return;
    }
    vec_pop(&dbg->prompt);
}


void dbg_ui_list_code(Debugger *dbg) {
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

void dbg_ui_list_tape(Debugger *dbg) {
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

void dbg_ui_help(Debugger *dbg) {
    printf(
        "Welcome to " FG_GREEN SET_ITALIC "C brainfuck debugger " RESET "help "
        "by %s\n"
        "\n"
        FG_GREEN "Usage:\n"
        FG_WHITE "  <command>\n" RESET
        "    Executes debugger command.\n"
        "\n"
        FG_WHITE "  :<command>\n" RESET
        "    Executes system command.\n"
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
        FG_YELLOW "  n  next  step " FG_DARK_GRAY "[N]" "\n" RESET
        "    Steps N instructions forward and prints the code and tape. \n"
        "    N = 1 by default.\n"
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
