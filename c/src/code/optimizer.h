#ifndef CODE_OPTIMIZER_H_INCLUDED
#define CODE_OPTIMIZER_H_INCLUDED

#include "instruction.h" // InstructionSteam

/// Creates stream that optimizes the instructions by accumulating them
/// together
InstructionStream o_acc_stream(InstructionStream is);

#endif // CODE_OPTIMIZER_H_INCLUDED
