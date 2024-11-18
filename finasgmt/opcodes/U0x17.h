#ifndef U0X17_H
#define U0X17_H

#include <stdio.h>

// Function to execute I-Type instructions
void execute_u17_type(int rd, int imm, int funct3, int *pc, int reg[])
{
    switch (funct3)
    {
    case 0x01: // AUIPC
        reg[rd] = *pc + (imm << 12);
        printf("Executed AUIPC: x%d = %x\n", rd, reg[rd]);
        break;
    default:
        printf("U-Type funct3=%x not implemented\n", funct3);
    }
}

#endif // U0X17_H
