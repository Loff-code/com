#ifndef I0X13_H
#define I0X13_H

#include <stdio.h>

// Function to execute I-Type instructions
void execute_i_type(int rd, int rs1, int imm, int funct3, int funct7, int reg[])
{
    int shamt = imm & 0x1f; // Extract the lower 5 bits of imm for shamt

    switch (funct3)
    {
    case 0x00:
        // Perform the ADDI operation
        reg[rd] = reg[rs1] + imm;
        printf("Executed ADDI: x%d = x%d + %d\n", rd, rs1, imm);
        break;
    case 0x01:
        // Perform the SLLI operation
        reg[rd] = reg[rs1] << shamt;
        printf("Executed SLLI: x%d = x%d << %d\n", rd, rs1, shamt);
        break;
    case 0x02:
        // Perform the SLTI operation
        reg[rd] = reg[rs1] < imm;
        printf("Executed SLTI: x%d = x%d < %d\n", rd, rs1, imm);
        break;
    case 0x03:
        // Perform the SLTIU operation
        reg[rd] = (uint32_t)reg[rs1] < imm;
        printf("Executed SLTIU: x%d = x%d < %d\n", rd, rs1, imm);
        break;
    case 0x04:
        // Perform the XORI operation
        reg[rd] = reg[rs1] ^ imm;
        printf("Executed XORI: x%d = x%d ^ %d\n", rd, rs1, imm);
        break;
    case 0x05:
        // Perform the SRLI operation
        if (funct7 == 0x00)
        {
            reg[rd] = (uint32_t)reg[rs1] >> shamt;
            printf("Executed SRLI: x%d = x%d >> %d\n", rd, rs1, shamt);
        }
        // Perform the SRAI operation
        else if (funct7 == 0x20)
        {
            reg[rd] = reg[rs1] >> shamt;
            printf("Executed SRAI: x%d = x%d >> %d\n", rd, rs1, shamt);
        }
        else
        {
            printf("funct7 = %x\n", funct7);
        }

        break;
    case 0x06:
        // Perform the ADDI operation
        reg[rd] = reg[rs1] | imm;
        printf("Executed ORI: x%d = x%d | %d\n", rd, rs1, imm);
        break;
    case 0x07:
        // Perform the ADDI operation
        reg[rd] = reg[rs1] & imm;
        printf("Executed ANDI: x%d = x%d & %d\n", rd, rs1, imm);
        break;
    default:
        printf("I-Type funct3=%x not implemented\n", funct3);
    }
    // Perform the ADDI operation
    // reg[rd] = reg[rs1] + imm;
    // printf("Executed ADDI: x%d = x%d + %d\n", rd, rs1, imm);
}

#endif // I0X13_H
