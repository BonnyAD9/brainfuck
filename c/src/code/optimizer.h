#ifndef OPTIMIZER_H_INCLUDED
#define OPTIMIZER_H_INCLUDED

#include "instruction.h" // InstructionSteam

/// Creates stream that optimizes the instructions by accumulating them
/// together
InstructionStream o_acc_stream(InstructionStream is);

#endif // OPTIMIZER_H_INCLUDED
