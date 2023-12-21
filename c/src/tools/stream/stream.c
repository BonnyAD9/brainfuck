#include "stream.h" // Stream

#include <stdlib.h> // free, NULL
#include <stdio.h>  // EOF

void s_default_free(Stream *s) {
    free(s->data);
    s->data = NULL;
}

void s_no_free(Stream *s) {
    s->data = NULL;
}

char *s_default_read(Stream *s, char *buf, size_t max_len) {
    for (int c; max_len-- && (c = s_get_chr(s)) != EOF; ++buf) {
        *buf = c;
    }
    return buf;
}

char *s_default_read_until(Stream *s, char *buf, size_t max_len, int end) {
    for (
        int c = end + 1;
        c != end && max_len-- && (c = s_get_chr(s)) != EOF;
        ++buf
    ) {
        *buf = c;
    }
    return buf;
}
