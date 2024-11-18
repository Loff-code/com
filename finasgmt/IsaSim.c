#include <stdio.h>
#include <stdint.h>
#include "R0x33.h" // R-Type instructions
#include "B0x63.h" // B-Type instructions
#include "I0x13.h" // I-Type instructions
#include "U0x17.h" // U-Type instructions
#include "U0x37.h" // U-Type instructions
#include "ecall.h" // U-Type instructions

int pc;
int reg[32];
int progr[] = {
    0x00100093,
    0x00008133,

};

// uint8_t memory4piece[] = {
//     00000000,
//     00000000,
//     00000000,
//     00000000,
// };

// for (size_t i = 0; i < 4; i++)
// {
//     uint32_t memory += memory4piece[i] << i * 8;
// }

int main()
{
    pc = 0;
    printf("Hello RISC-V World!\n");

    while (1)
    {
        int instr = progr[pc >> 2]; // Fetch instruction
        int opcode = instr & 0x7f;  // Extract opcode
        int rd = (instr >> 7) & 0x1f;
        int rs1 = (instr >> 15) & 0x1f;
        int rs2 = (instr >> 20) & 0x1f;
        int imm = (instr >> 20) & 0xfff;
        int funct3 = (instr >> 12) & 0x7;
        int funct7 = (instr >> 25) & 0x7f;

        if (imm & 0x800)
            imm |= 0xfffff000; // Sign-extend immediate

        switch (opcode)
        {
        // case 0x1b: // I-Type
        //     execute_i1b_type(rd, rs1, imm, funct3, funct7, reg);
        //     printf("Opcode %x\n", opcode);
        //     break;
        case 0x13: // I-Type
            execute_i_type(rd, rs1, imm, funct3, funct7, reg);
            printf("Opcode %x\n", opcode);
            break;
        case 0x17: // U-Type
            execute_u17_type(rd, instr >> 12, funct3, &pc, reg);
            printf("Opcode %x\n", opcode);
            break;
        case 0x33: // R-Type
            execute_r_type(rd, rs1, rs2, funct3, funct7, reg);
            printf("Opcode %x\n", opcode);
            break;
        case 0x37: // U-Type
            execute_u_type(rd, rs1, instr >> 12, funct3, reg);
            printf("Opcode %x\n", opcode);
            break;
        case 0x63: // B-Type
            execute_b_type(rs1, rs2, instr, funct3, &pc, reg);
            printf("Opcode %x\n", opcode);
            // break;
            continue; // Skip the pc increment for branching
        case 0x73:
            execute_ecall(rd, rs1, imm, funct3, funct7, reg);
            printf("Opcode %x\n", opcode);
            break;
        default:
            if (opcode == 0x1b)
            {
                opcode = 0x13;
            }
            else
            {
                printf("Opcode %x not implemented\n", opcode);
            }
        }

        pc += 4; // Increment program counter
        if ((pc >> 2) >= sizeof(progr) / sizeof(progr[0]))
            break;

        // Print register states after each instruction
        for (int i = 0; i < sizeof(reg) / sizeof(reg[0]); ++i)
        {
            if (reg[i] != 0)
            {
                printf("x%d=%d ", i, reg[i]);
            }
        }
        printf("\n");
    }

    // Final state
    printf("Final register states:\n");
    for (int i = 0; i < sizeof(reg) / sizeof(reg[0]); ++i)
    {
        if (reg[i] != 0)
        {
            printf("x%d=%d ", i, reg[i]);
        }
    }
    printf("\n");

    return 0;
}
