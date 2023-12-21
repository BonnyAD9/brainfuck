#ifndef C_TRANSPILER_INCLUDED
#define C_TRANSPILER_INCLUDED

#include <stdio.h>   // FILE
#include <stdbool.h> // bool

#include "../tools/vec.h" // Vec

void c_transpile(FILE *out, const Vec instructions, size_t tape_size);

#endif // C_TRANSPILER_INCLUDED
