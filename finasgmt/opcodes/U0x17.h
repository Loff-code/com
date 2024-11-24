#ifndef U0X17_H
#define U0X17_H

#include <stdio.h>

// Function to execute I-Type instructions
void execute_u17_type(int rd, int imm, int *pc, int reg[])
{
    reg[rd] = *pc + (imm << 12);
    printf("Executed AUIPC: x%d = %x\n", rd, reg[rd]);
}

#endif // U0X17_H
