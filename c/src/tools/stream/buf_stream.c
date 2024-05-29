#include "buf_stream.h"

#include <stdlib.h> // free
#include <stdio.h>  // EOF

#include "../vec.h" // Vec, vec_free, vec_clear
#include "stream.h" // s_free

BufStream *bs_new(Stream input, int buf_char) {
    BufStream *res = malloc(sizeof(*res));
    if (!res) {
        return NULL;
    }

    *res = (BufStream) {
        .buf_char = buf_char,
        .idx = 0,
        .data = VEC_NEW(char),
        .input = input,
        .eof = false,
    };

    return res;
}

Stream bs_to_stream(BufStream *bs) {
    return (Stream) {
        .data = bs,
        .free = bss_free,
        .get_chr = bss_get_chr,
        .read = s_default_read,
        .read_until = s_default_read_until,
    };
}

void bs_free(BufStream **s) {
    BufStream *v = *s;
    *s = NULL;
    vec_free(&v->data);
    s_free(&v->input);
    free(v);
}

int bs_get_char(BufStream *s) {
    if (s->idx < s->data.len) {
        return s->data.data[s->idx++];
    }

    if (s->eof) {
        s->eof = false;
        return EOF;
    }

    vec_clear(&s->data);
    int c = s->buf_char + 1;
    while (c != s->buf_char && (c = s_get_chr(&s->input)) != EOF) {
        VEC_PUSH(char, &s->data, c);
    }

    if (c == EOF) {
        s->eof = true;
    }

    if (s->data.len == 0) {
        s->idx = 0;
        s->eof = false;
        return EOF;
    }

    s->idx = 1;
    return s->data.data[0];
}

void bss_free(Stream *s) {
    bs_free((BufStream **)&s->data);
}

int bss_get_chr(Stream *s) {
    return bs_get_char((BufStream *)s->data);
}
