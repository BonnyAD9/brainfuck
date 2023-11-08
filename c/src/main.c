#include <stdio.h>  // size_t, FILE, stdin, fgetc, EOF
#include <stdlib.h> // malloc, free, realloc, EXIT_FAILRE

/// Reads all bytes from the stream
char *read_all_bytes(FILE *f, size_t *len);
void interpret(char *code, size_t code_len, char *tape, size_t tape_len);

int main(int argc, char **argv) {
    if (argc != 2) {
        return EXIT_FAILURE;
    }
    const size_t tape_size = 30000;
    FILE *in = fopen(argv[1], "r");

    char *tape = calloc(tape_size, sizeof(*tape));
    if (!tape) {
        return EXIT_FAILURE;
    }

    size_t code_len;
    char *code = read_all_bytes(in, &code_len);
    fclose(in);
    if (!code) {
        free(tape);
        return EXIT_FAILURE;
    }

    interpret(code, code_len, tape, tape_size);

    free(tape);
    free(code);
}

char *read_all_bytes(FILE *f, size_t *len) {
    char *res = malloc(sizeof(*res));
    *len = 0;
    size_t alloced = 1;
    if (!res) {
        return NULL;
    }

    int cur;
    while ((cur = fgetc(f)) != EOF) {
        if (*len == alloced) {
            size_t new_len = alloced * 2;
            char *new_res = realloc(res, sizeof(*new_res) * new_len);
            if (!new_res) {
                free(res);
                return NULL;
            }
            res = new_res;
            alloced = new_len;
        }
        res[*len] = cur;
        ++*len;
    }

    return res;
}

void interpret(char *code, size_t code_len, char *tape, size_t tape_len) {
    size_t ci = 0;
    size_t ti = 0;

    while (ci < code_len && ti < tape_len) {
        switch (code[ci]) {
        case '>':
            ++ti;
            break;
        case '<':
            --ti;
            break;
        case '+':
            ++tape[ti];
            break;
        case '-':
            --tape[ti];
            break;
        case '.':
            putchar(tape[ti]);
            fflush(stdout);
            break;
        case ',':
            tape[ti] = getchar();
            break;
        case '[':
            if (tape[ti]) {
                break;
            }
            while (ci < code_len && code[ci] != ']') {
                ++ci;
            }
            break;
        case ']':
            if (!tape[ti]) {
                break;
            }
            while (ci < code_len && code[ci] != '[') {
                --ci;
            }
            break;
        }
        ++ci;
    }
}
