#ifndef ARG_PARSER_H_INCLUDED
#define ARG_PARSER_H_INCLUDED

#include <stdio.h> // size_t, FILE

typedef enum {
    INTERPRET,
    TRANSPILE,
    DEBUG,
} ArgAction;

typedef struct {
    ArgAction action;
    char *file;
    char *output;
    size_t tape_size;
    unsigned print_info: 1;
    unsigned print_help: 1;
} Args;

Args arg_parse(char **argv);

void args_print(Args *args, FILE *out);

#endif // ARG_PARSER_H_INCLUDED
