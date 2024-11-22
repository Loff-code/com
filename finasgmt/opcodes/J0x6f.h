#ifndef J0x6f_h
#define J0x6f_h

#include <stdio.h>

// Function to execute I-Type instructions
void execute_j_type(int rd, int rs1, int imm, int funct3, int funct7, int reg[], int *pc)
{
    // switch (funct3)
    // {
    // case 0x00:
    reg[rd] = *pc + 4;
    printf("Executed JAL: jump and x1 = initial + 4 || x%d = %d  imm%d pc%d\n", rd, reg[rd], imm, pc);

    // printf("%d\n\n\n", imm);
    *pc += imm - 4;

    // return;

    // // printf("J-Type funct3=%x not implemented\n", funct3);
    // break;

    // default:
    //     printf("J-Type funct3=%x not implemented\n", funct3);
    // }
}

#endif // J0x6f_h
