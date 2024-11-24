#ifndef I0X67_H
#define I0X67_H

#include <stdio.h>

// Function to execute I-Type instructions
void execute_i0x67_type(int rd, int rs1, int imm, int reg[], int *pc)
{
    printf("Executed JALR: ");
    if (rd != 0)
    {
        reg[rd] = *pc + 4;
        printf("x%d = %d ", rd, *pc + 4);
    }
    else
    {
        printf("x%d storing skipped, ", rd);
    }
    *pc = imm + reg[rs1] - 4;
    printf("jump to pc:%d\n", *pc + 4);
}

#endif // I0X67_H