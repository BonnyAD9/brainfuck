#include "c-transpiler.h"

#include <stdio.h>   // FILE, fputc, fprintf
#include <stdbool.h> // bool

#include "vec.h" // Vec
#include "instruction.h"

static void transpile_instruction(FILE *out, size_t *indent, Instruction inst);
static void repeat(FILE *out, char c, size_t amount);

void c_transpile(FILE *out, const Vec instructions, size_t tape_size) {
    fprintf(
        out,
        "#include <stdio.h>  // getchar, putchar, size_t, fprintf, stderr\n"
        "#include <stdlib.h> // calloc, free, EXIT_FAILURE\n"
        "\n"
        "#define ESC \"\\x1b\"\n"
        "#define CSI ESC \"[\"\n"
        "#define FG_RED \"91m\""
        "\n"
        "int main() {\n"
        "    const size_t tape_size = %zu;\n"
        "    unsigned char *tape = calloc(tape_size, sizeof(char));\n"
        "    if (!tape) {\n"
        "        fprintf(\n"
        "            stderr,\n"
        "            FG_RED \"error\" \": failed to allocate %%zu bytes\",\n"
        "            tape_size\n"
        "        );\n"
        "    }\n"
        "    size_t idx = 0;\n"
        "\n",
        tape_size
    );

    size_t indentation = 4;

    VEC_FOR_EACH(&instructions, Instruction, inst) {
        transpile_instruction(out, &indentation, *inst.v);
    }

    fprintf(
        out,
        "    free(tape);\n"
        "}\n"
    );
}

static void transpile_instruction(FILE *out, size_t *indent, Instruction inst) {
    if (inst.move) {
        repeat(out, ' ', *indent);
        fprintf(out, "idx = (idx + %ld) %% tape_size;\n", inst.move);
    }

    if (inst.add) {
        repeat(out, ' ', *indent);
        fprintf(out, "tape[idx] += %ld;\n", inst.add);
    }

    if (inst.flags & INST_PRINT) {
        repeat(out, ' ', *indent);
        fprintf(out, "putchar(tape[idx]);\n");
    }

    if (inst.flags & INST_READ) {
        repeat(out, ' ', *indent);
        fprintf(out, "tape[idx] = getchar();\n");
    }

    if (inst.jump > 0) {
        repeat(out, ' ', *indent);
        fprintf(out, "while (tape[idx]) {\n");
        *indent += 4;
    }

    if (inst.jump < 0) {
        *indent -= 4;
        repeat(out, ' ', *indent);
        fprintf(out, "}\n");
    }

    fprintf(out, "\n");
}

static void repeat(FILE *out, char c, size_t amount) {
    while (amount--) {
        fputc(c, out);
    }
}
