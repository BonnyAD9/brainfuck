#include <stdio.h>  // getchar, putchar, size_t, fprintf, stderr
#include <stdlib.h> // calloc, free, EXIT_FAILURE

#define ESC "\x1b"
#define CSI ESC "["
#define FG_RED "91m"
int main() {
    const size_t tape_size = 30000;
    unsigned char *tape = calloc(tape_size, sizeof(char));
    if (!tape) {
        fprintf(
            stderr,
            FG_RED "error" ": failed to allocate %zu bytes",
            tape_size
        );
    }
    size_t idx = 0;

    idx = (idx + 5) % tape_size;
    tape[idx] = getchar();

    tape[idx] += 1;
    while (tape[idx]) {

        idx = (idx + 3) % tape_size;
        tape[idx] = getchar();

        tape[idx] += 1;
    }

    idx = (idx + -3) % tape_size;
    while (tape[idx]) {

        idx = (idx + -3) % tape_size;
        while (tape[idx]) {

            idx = (idx + 3) % tape_size;
            while (tape[idx]) {

                tape[idx] += -1;

                idx = (idx + -3) % tape_size;
                tape[idx] += -1;

                idx = (idx + -1) % tape_size;
                tape[idx] += 1;

                idx = (idx + 1) % tape_size;
                while (tape[idx]) {

                    idx = (idx + 1) % tape_size;
                }

                idx = (idx + 2) % tape_size;
            }

            idx = (idx + -3) % tape_size;
            while (tape[idx]) {

                idx = (idx + -1) % tape_size;
            }

            idx = (idx + 2) % tape_size;
            while (tape[idx]) {

                idx = (idx + 3) % tape_size;
                tape[idx] += 1;

                idx = (idx + -3) % tape_size;
                tape[idx] += -1;
            }

            idx = (idx + -1) % tape_size;
            while (tape[idx]) {

                idx = (idx + 1) % tape_size;
                tape[idx] += 1;

                idx = (idx + 3) % tape_size;
                tape[idx] += 1;

                idx = (idx + -4) % tape_size;
                tape[idx] += -1;
            }

            idx = (idx + -2) % tape_size;
        }

        idx = (idx + 3) % tape_size;
        while (tape[idx]) {

            tape[idx] += -1;
            putchar(tape[idx]);
            while (tape[idx]) {

                tape[idx] += -1;
            }

        }

        idx = (idx + 3) % tape_size;
        while (tape[idx]) {

            idx = (idx + 3) % tape_size;
        }

        idx = (idx + -3) % tape_size;
    }

    tape[idx] += 10;
    putchar(tape[idx]);

    free(tape);
}
