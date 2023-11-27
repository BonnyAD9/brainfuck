#include "debug-parser.h"

#include <string.h> // strcmp

#include "debugger.h" // Debugger
#include "err.h"      // print_err

DbgCmd dbg_parse_cmd(Debugger *dbg) {
    DbgCmd res = { .action = DA_NONE, };

    char *str = dbg->prompt.data;

    if (*str == 0) {
        return res;
    }

    if (*str == ':') {
        res.action = DA_SYSTEM;
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
