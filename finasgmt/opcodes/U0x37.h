#ifndef U0X37_H
#define U0X37_H

#include <stdio.h>

// Function to execute I-Type instructions
void execute_u_type(int rd, int rs1, int imm, int funct3, int reg[])
{
    switch (funct3)
    {
    case 0x00: // LUI
        reg[rd] = imm << 12;
        printf("Executed LUI: x%d = %x\n", rd, reg[rd]);
        break;
    // case 0x1b: // LUI
    //     reg[rd] = reg[rs1] + imm;
    //     printf("Executed LUI: x%d = %x\n", rd, reg[rd]);
    //     break;
    case 0x02: // LUI
        reg[rd] = imm << 12;
        printf("Executed LUI: x%d = %x\n", rd, reg[rd]);
        break;
    case 0x05: // LUI
        reg[rd] = imm << 12;
        printf("Executed LUI: x%d = %x\n", rd, reg[rd]);
        break;
    case 0x07: // LUI
        reg[rd] = imm << 12;
        printf("Executed LUI: x%d = %x\n", rd, reg[rd]);
        break;
    default:
        printf("U-Type funct3=%x not implemented\n", funct3);
    }
}

#endif // U0X37_H
