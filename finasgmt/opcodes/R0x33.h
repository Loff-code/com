#ifndef R0X33_H
#define R0X33_H
#include <stdio.h>

void execute_r_type(int rd, int rs1, int rs2, int funct3, int funct7, int reg[])
{
    switch (funct3)
    {
    case 0x07: // AND
        reg[rd] = reg[rs1] & reg[rs2];
        printf("Executed AND: x%d r= x%d & x%d\n", rd, rs1, rs2);
        break;
    case 0x06: // OR
        reg[rd] = reg[rs1] | reg[rs2];
        printf("Executed OR: x%d = x%d | x%d\n", rd, rs1, rs2);
        break;
    case 0x05: // SRL and SRA
        if (funct7 == 0x00)
        {
            reg[rd] = (uint32_t)reg[rs1] >> reg[rs2];
            printf("Executed SRL: x%d = x%d >> x%d\n", rd, rs1, rs2);
        }
        else if (funct7 == 0x20)
        {
            reg[rd] = reg[rs1] >> reg[rs2];
            printf("Executed SRA: x%d = (UNSIGNED)x%d >> x%d\n", rd, rs1, rs2);
        }
        else
        {
            printf("funct7 = %x\n", funct7);
        }

        break;
    case 0x04: // XOR
        reg[rd] = reg[rs1] ^ reg[rs2];
        printf("Executed XOR: x%d = x%d ^ x%d\n", rd, rs1, rs2);
        break;
    case 0x03: // SLTU
        reg[rd] = (uint32_t)reg[rs1] < (uint32_t)reg[rs2];
        printf("Executed SLTU: x%d = (uint32_t)x%d < (uint32_t)x%d\n", rd, rs1, rs2);
        break;
    case 0x02: // SLT
        reg[rd] = reg[rs1] < reg[rs2];
        printf("Executed SLT: x%d = x%d < x%d\n", rd, rs1, rs2);
        break;
    case 0x01: // SLL
        reg[rd] = reg[rs1] << reg[rs2];
        printf("Executed SLL: x%d = x%d << x%d\n", rd, rs1, rs2);
        break;
    case 0:                 // ADD AND SUB
        if (funct7 == 0x00) // ADD
        {
            reg[rd] = reg[rs1] + reg[rs2];
            printf("Executed ADD: x%d = x%d + x%d\n", rd, rs1, rs2);
        }
        else if (funct7 == 0x20) // SUB
        {
            reg[rd] = reg[rs1] - reg[rs2];
            printf("Executed SUB: x%d = x%d - x%d\n", rd, rs1, rs2);
        }
        else
        {
            printf("funct7 = %x\n", funct7);
        }

        break;
    default:
        printf("R-Type funct3=%x not implemented\n", funct3);
    }
}

#endif // R0X33_H
