#ifndef TOOLS_STREAM_BUF_STREAM_H_INCLUDED
#define TOOLS_STREAM_BUF_STREAM_H_INCLUDED

#include "../vec.h"
#include "stream.h"

typedef struct {
    int buf_char;
    size_t idx;
    Vec data;
    Stream input;
} BufStream;

BufStream *bs_new(Stream input, int buf_char);

Stream bs_to_stream(BufStream *bs);

void bs_free(BufStream **s);

int bs_get_char(BufStream *s);

void bss_free(Stream *s);

int bss_get_chr(Stream *s);

#endif // TOOLS_STREAM_BUF_STREAM_H_INCLUDED
