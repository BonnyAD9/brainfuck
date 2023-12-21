#include "file_stream.h"

#include <stdio.h> // fopen, fclose, FILE, fgetc

Stream open_file_stream(const char *file, const char *mode) {
    return (Stream) {
        .data = fopen(file, mode),
        .free = fs_free,
        .get_chr = fs_get_chr,
        .read = s_default_read,
        .read_until = s_default_read_until,
    };
}

void fs_free(Stream *s) {
    if (s->data) {
        fclose((FILE *)s->data);
        s->data = NULL;
    }
}

int fs_get_chr(Stream *s) {
    return fgetc((FILE *)s->data);
}

char *fs_read(Stream *s, char *buf, size_t max_len) {
    return buf + fread(buf, 1, max_len, (FILE *)s->data);
}

char *fs_read_until(Stream *s, char *buf, size_t max_len, int end) {
    for (
        int c = end + 1;
        c != end && max_len-- && (c = fgetc((FILE *)s->data)) != EOF;
        ++buf
    ) {
        *buf = c;
    }
    return buf;
}
