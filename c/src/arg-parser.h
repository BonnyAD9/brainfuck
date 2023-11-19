#ifndef ARG_PARSER_H_INCLUDED
#define ARG_PARSER_H_INCLUDED

#include <stdio.h> // size_t, FILE

typedef struct {
    char *file;
    size_t tape_size;
    int print_info: 1;
} Args;

Args arg_parse(char **argv);

void args_print(Args *args, FILE *out);

#endif // ARG_PARSER_H_INCLUDED
