#ifndef B0X63_H
#define B0X63_H

#include <stdio.h>

void execute_b_type(int rs1, int rs2, int imm, int funct3, int *pc, int reg[])
{

    switch (funct3)
    {
    case 0x0: // BEQ (Branch if Equal)
        if (reg[rs1] == reg[rs2])
        {
            *pc += imm;
            printf("Executed BEQ: %d == %d, branching to pc=%d\n", reg[rs1], reg[rs2], *pc);
            return;
        }
        else
        {
            printf("BEQ not taken: %d != %d, moving to pc=%d\n", reg[rs1], reg[rs2], *pc + 4);
        }
        break;

    case 0x1: // BNE (Branch if Not Equal)
        if (reg[rs1] != reg[rs2])
        {
            *pc += imm;
            printf("Executed BNE: %d != %d, branching to pc=%d\n", reg[rs1], reg[rs2], *pc);
            return;
        }
        else
        {
            printf("BNE not taken: %d == %d, moving to pc=%d\n", reg[rs1], reg[rs2], *pc + 4);
        }
        break;

    case 0x4: // BLT (Branch if Less Than)
        if ((int)reg[rs1] < (int)reg[rs2])
        {
            *pc += imm;
            printf("Executed BLT: %d < %d, branching to pc=%d\n", reg[rs1], reg[rs2], *pc);
            return;
        }
        else
        {
            printf("BLT not taken: %d >= %d, moving to pc=%d\n", reg[rs1], reg[rs2], *pc + 4);
        }
        break;

    case 0x5: // BGE (Branch if Greater or Equal)
        if ((int)reg[rs1] >= (int)reg[rs2])
        {
            *pc += imm;
            printf("Executed BGE: %d >= %d, branching to pc=%d\n", reg[rs1], reg[rs2], *pc);
            return;
        }
        else
        {
            printf("BGE not taken: %d < %d, moving to pc=%d\n", reg[rs1], reg[rs2], *pc + 4);
        }
        break;

    case 0x6: // BLTU (Branch if Less Than Unsigned)
        if ((unsigned int)reg[rs1] < (unsigned int)reg[rs2])
        {
            *pc += imm;
            printf("Executed BLTU: %u < %u, branching to pc=%d\n", reg[rs1], reg[rs2], *pc);
            return;
        }
        else
        {
            printf("BLTU not taken: %u >= %u, moving to pc=%d\n", reg[rs1], reg[rs2], *pc + 4);
        }
        break;

    case 0x7: // BGEU (Branch if Greater or Equal Unsigned)
        if ((unsigned int)reg[rs1] >= (unsigned int)reg[rs2])
        {
            *pc += imm;
            printf("Executed BGEU: %u >= %u, branching to pc=%d\n", reg[rs1], reg[rs2], *pc);
            return;
        }
        else
        {
            printf("BGEU not taken: %u < %u, moving to pc=%d\n", reg[rs1], reg[rs2], *pc + 4);
        }
        break;

    default:
        printf("B-Type funct3=%x not implemented\n", funct3);
    }

    // Default case: No branch taken, move to the next instruction
    *pc += 4;
}

#endif // B0X63_H
