#ifndef TOOLS_STREAM_FILE_STREAM_H_INCLUDED
#define TOOLS_STREAM_FILE_STREAM_H_INCLUDED

#include "stream.h"

#include <stdio.h> // stdin

#define FS_STDIN ((Stream) { \
    .data = stdin, \
    .free = s_no_free, \
    .get_chr = fs_get_chr, \
    .read = fs_read, \
    .read_until = fs_read_until, \
})

Stream open_file_stream(const char *file, const char *mode);

void fs_free(Stream *s);

int fs_get_chr(Stream *s);

char *fs_read(Stream *s, char *buf, size_t max_len);

char *fs_read_until(Stream *s, char *buf, size_t max_len, int end);

#endif // TOOLS_STREAM_FILE_STREAM_H_INCLUDED
