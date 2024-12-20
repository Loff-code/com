#ifndef J0x6f_h
#define J0x6f_h

#include <stdio.h>

// Function to execute I-Type instructions
void execute_j_type(int rd, int rs1, int imm, int reg[], int *pc)
{
    printf("Executed JAL: ");

    if (rd != 0)
    {
        reg[rd] = *pc + 4;
        printf("x%d = %d, ", rd, *pc + 4);
    }
    else
    {
        printf("x%d storing skipped, ", rd);
    }

    *pc += imm - 4;
    printf("jump to pc:%d\n", *pc + 4);
}

#endif // J0x6f_h
