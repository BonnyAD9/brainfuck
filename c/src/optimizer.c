#include "optimizer.h"

#include <stdlib.h>  // malloc, free
#include <stdbool.h> // bool, true, false

#include "instruction.h" // InstructionSteam, Instruction, InstFlags::*
#include "err.h"         // set_err_msg, FAILED_ALLOC

typedef struct {
    InstructionStream is;
    Instruction cur;
} AccInstStream;

static Instruction acc_stream_next(AccInstStream *is);
static void acc_stream_free(AccInstStream *is);
static bool acc_try_join(Instruction *first, Instruction second);

InstructionStream o_acc_stream(InstructionStream is) {
    AccInstStream *ais = malloc(sizeof(*ais));
    if (!ais) {
        is.free(is.data);
        set_err_msg(FAILED_ALLOC, "Failed to allocate AccInstStream");
        return (InstructionStream) {};
    }

    *ais = (AccInstStream) {
        .is = is,
        .cur = instruction_new(0, 0, INST_NONE, 0),
    };

    return (InstructionStream) {
        .data = ais,
        .next = (InstructionStreamNextFun)acc_stream_next,
        .free = (FreeFun)acc_stream_free,
    };
}

static Instruction acc_stream_next(AccInstStream *is) {
    Instruction ret = is->cur;
    is->cur = is->is.next(is->is.data);
    while (acc_try_join(&ret, is->cur)) {
        is->cur = is->is.next(is->is.data);
    }
    return ret;
}

static void acc_stream_free(AccInstStream *is) {
    is->is.free(is->is.data);
    free(is);
}

static bool acc_try_join(Instruction *first, Instruction second) {
    if (first->flags == INST_NONE) {
        *first = second;
        return true;
    }

    if (second.flags & INST_MOVE && !(second.flags & ~INST_MOVE)) {
        if (first->flags & ~INST_MOVE) {
            return false;
        }

        first->flags |= INST_MOVE;
        first->move += second.move;
        return true;
    }

    if (second.flags & INST_ADD && !(second.flags & ~INST_ADD)) {
        if (first->flags & ~(INST_MOVE | INST_ADD)) {
            return false;
        }

        first->flags |= INST_ADD;
        first->add += second.add;
        return true;
    }

    if (second.flags & INST_PRINT && !(second.flags & ~INST_PRINT)) {
        if (first->flags & ~(INST_MOVE | INST_ADD)) {
            return false;
        }

        first->flags |= INST_PRINT;
        return true;
    }

    if (second.flags & INST_READ && !(second.flags & ~INST_READ)) {
        if (first->flags & ~(INST_MOVE | INST_ADD | INST_PRINT)) {
            return false;
        }

        first->flags |= INST_READ;
        return true;
    }

    if (second.flags & INST_JUMP && !(second.flags & ~(INST_JUMP))) {
        if (first->flags & ~(INST_MOVE | INST_ADD | INST_PRINT | INST_READ)) {
            return false;
        }

        first->flags |= second.flags;
        first->jump = second.jump;
        return true;
    }

    return false;
}
