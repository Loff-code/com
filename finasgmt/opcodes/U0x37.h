#ifndef U0X37_H
#define U0X37_H

#include <stdio.h>

// Function to execute I-Type instructions
void execute_u_type(int rd, int imm, int reg[])
{
    reg[rd] = imm << 12;
    printf("Executed LUI: x%d = %x\n", rd, reg[rd]);
}

#endif // U0X37_H
