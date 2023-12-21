#include "parsers.h"

#include <stdlib.h> // size_t, strtoul, strtoull

#include "err.h" // INVALID_ARGS, IS_ERR, set_err_msg

size_t parse_size(char *str) {
    if (!str) {
        set_err_msg(INVALID_ARGS, "Missing unsigned number argument.");
        return 0;
    }

    if (sizeof(size_t) == sizeof(long long)) {
        char *end;
        size_t res = strtoull(str, &end, 10);
        if (IS_ERR) {
            return 0;
        }
        if (*end || end == str) {
            set_err_msg(INVALID_ARGS, "Invalid unsigned number.");
            return 0;
        }
        return res;
    }

    char *end;
    size_t res = strtoul(str, &end, 10);
    if (IS_ERR) {
        return 0;
    }
    if (*end || end == str) {
        set_err_msg(INVALID_ARGS, "Invalid unsigned number");
        return 0;
    }
    return res;
}
