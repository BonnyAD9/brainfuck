#ifndef TOOLS_ERR_H_INCLUDED
#define TOOLS_ERR_H_INCLUDED

#include <stdio.h> // fprintf, stderr, FILE

#include "ansi-terminal.h" // FG_*, RESET

/// No error code
#define NO_ERROR 0
/// Generic error code
#define GENERIC_ERR -1
/// Failed to allocate
#define FAILED_ALLOC -2
/// Invalid command line arguments
#define INVALID_ARGS -3

/// True if any error occured, otherwise false. Clears the error
#define POP_ERR (pop_err(NULL) != NO_ERROR)
/// False if any error occured, otherwise true. Clears the error
#define POP_OK (pop_err() == NO_ERROR)

/// True if any error occured, otherwise false.
#define IS_ERR (get_err() != NO_ERROR)
/// False if any error occured, otherwise true.
#define IS_OK (get_err() != NO_ERROR)

/// Prints to the stderr, also adds newline
#define EPRINTF(fmt, ...) fprintf(stderr, fmt "\n" ,##__VA_ARGS__)

#define WPRINTF(fmt, ...) fprintf( \
    stderr, \
    FG_MAGENTA "warning" RESET ": " fmt "\n" \
    ,##__VA_ARGS__ \
)

/// If error is set, jump to the given label.
#define EPROP(ret) if (IS_ERR) goto ret

/// clears the error
void clear_err(void);

/// returns the last error code and clears the error. The returned msg is never
/// NULL.
int pop_err(const char **msg);

/// Sets the error to a generic error
void set_err(void);

/// Sets the error message. The msg must be static string.
void set_err_msg(int code, const char *msg);

/// Gets the error code-
int get_err(void);

/// Gets the error message. This is never NULL.
const char *get_err_msg(void);

/// If there is error prints it. Returns the error code.
int print_err(const char *str, ...);

void err_print_info(FILE *out);

#endif // TOOLS_ERR_H_INCLUDED
