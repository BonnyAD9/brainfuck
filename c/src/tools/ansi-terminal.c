#include "ansi-terminal.h"

void repeat(FILE *out, char c, size_t amount) {
    while (amount--) {
        fputc(c, out);
    }
}
