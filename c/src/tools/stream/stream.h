#ifndef TOOLS_STREAM_STREAM_H_INCLUDED
#define TOOLS_STREAM_STREAM_H_INCLUDED

#include <stdbool.h> // bool
#include <stddef.h>  // size_t

typedef struct Stream Stream;

typedef void (*SFreeFun)(Stream *s);
typedef int (*SGetChr)(Stream *s);
typedef char *(*SRead)(Stream *s, char *buf, size_t max_len);
typedef char *(*SReadUntil)(Stream *s, char *buf, size_t max_len, int end);

struct Stream {
    void *data;
    SFreeFun free;
    SGetChr get_chr;
    SRead read;
    SReadUntil read_until;
};

/// Frees the stream
static inline void s_free(Stream *s) {
    s->free(s);
}

/// Gets the next char in the stream, returns EOF if there is no data.
static inline int s_get_chr(Stream *s) {
    return s->get_chr(s);
}

/// Reads up to `max_len` characters from the stream.
///
/// Returns the first unmodified position in `buf`
static inline char *s_read(Stream *s, char *buf, size_t max_len) {
    return s->read(s, buf, max_len);
}

/// Reads up to `max_len` characters from the stream until a character `end`.
///
/// Returns the first unmodified position in `buf`
static inline char *s_read_until(
    Stream *s,
    char *buf,
    size_t max_len,
    int end
) {
    return s->read_until(s, buf, max_len, end);
}

void s_default_free(Stream *s);

void s_no_free(Stream *s);

char *s_default_read(Stream *s, char *buf, size_t max_len);

char *s_default_read_until(Stream *s, char *buf, size_t max_len, int end);

#endif // TOOLS_STREAM_STREAM_H_INCLUDED
