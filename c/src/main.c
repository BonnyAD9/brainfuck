#include <stdio.h>  // size_t, FILE, stdin, fgetc, EOF
#include <stdlib.h> // malloc, free, realloc, EXIT_FAILRE

#include "vec.h" // Vec, VEC_NEW
#include "err.h" // IS_ERR, print_err, EPRINTF

/// Reads all bytes from the stream
Vec read_all_bytes(FILE *f);
void interpret(const Vec code, const Vec tape);

int main(int argc, char **argv) {
    if (argc != 2) {
        EPRINTF("Invalid number of arguments.");
        return EXIT_FAILURE;
    }

    FILE *in = fopen(argv[1], "r");
    if (!in) {
       EPRINTF("Failed to open file %s", argv[1]);
       return EXIT_FAILURE;
    }

    const size_t tape_size = 30000;
    Vec tape = VEC_NEW(char);
    VEC_EXTEND_EXACT(char, &tape, tape_size, 0);

    Vec code = read_all_bytes(in);
    fclose(in);
    if (IS_ERR) {
        return print_err();
    }

    interpret(code, tape);

    vec_free(&code);
    vec_free(&tape);
}

Vec read_all_bytes(FILE *f) {
    Vec res = VEC_NEW(char);

    int cur;
    while ((cur = fgetc(f)) != EOF) {
        VEC_PUSH(char, &res, cur);
        if (IS_ERR) {
            vec_free(&res);
            return VEC_NEW(char);
        }
    }

    return res;
}

void interpret(const Vec code, const Vec tape) {
    size_t ci = 0;
    size_t ti = 0;

    while (ci < code.len && ti < tape.len) {
        switch (VEC_AT(char, code, ci)) {
        case '>':
            ti = (ti + 1) % tape.len;
            break;
        case '<':
            ti = (ti - 1) % tape.len;
            break;
        case '+':
            ++VEC_AT(char, tape, ti);
            break;
        case '-':
            --VEC_AT(char, tape, ti);
            break;
        case '.':
            putchar(VEC_AT(char, tape, ti));
            break;
        case ',': {
            int chr = getchar();
            VEC_AT(char, tape, ti) = chr;
            break;
        }
        case '[':
            if (VEC_AT(char, tape, ti)) {
                break;
            }
            for (size_t i = 1; i && ++ci < code.len; ) {
                switch (VEC_AT(char, code, ci))
                {
                case '[':
                    ++i;
                    break;
                case ']':
                    --i;
                    break;
                }
            }
            break;
        case ']':
            if (!VEC_AT(char, tape, ti)) {
                break;
            }
            for (size_t i = 1; i && --ci < code.len; ) {
                switch (VEC_AT(char, code, ci))
                {
                case '[':
                    --i;
                    break;
                case ']':
                    ++i;
                    break;
                }
            }
            break;
        }
        ++ci;
    }
}
