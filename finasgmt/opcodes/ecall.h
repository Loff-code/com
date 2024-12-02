#ifndef ecall_h
#define ecall_h

#include <stdio.h>

// Function to execute I-Type instructions
void execute_ecall(int rd, int rs1, int imm, int funct3, int funct7, int reg[])
{
    switch (funct3)
    {
    case 0x00:
        // Perform the ADDI operation
        reg[rd] = reg[rs1] + imm;
        printf("Executed ADDI: x%d = x%d + %d\n", rd, rs1, imm);
        break;

    default:
        printf("ECALL funct3=%x not implemented\n", funct3);
    }
}

#endif // ECALL
