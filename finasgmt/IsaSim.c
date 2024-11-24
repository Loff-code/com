#include "funcs/funcs.h"

int main(int argc, char *argv[])
{
    printf("Hello RISC-V World!\n");
    if (argc < 2)
    {
        printf("Usage: %s <program.bin>\n", argv[0]);
        return 1;
    }

    load_bin_file(argv[1]);

    while (1)
    {

        // if (i > 100)
        // {
        //     printf("\n");
        //     return 0;
        // }

        int instr = memory[pc >> 2]; // Fetch instruction
        printf("Instruction: 0x%08X\n", instr);

        uint8_t opcode = instr & 0x7F;         // Bits [6:0]
        uint8_t rd = (instr >> 7) & 0x1F;      // Bits [11:7]
        uint8_t funct3 = (instr >> 12) & 0x7;  // Bits [14:12]
        uint8_t rs1 = (instr >> 15) & 0x1F;    // Bits [19:15]
        uint8_t rs2 = (instr >> 20) & 0x1F;    // Bits [24:20]
        uint8_t funct7 = (instr >> 25) & 0x7F; // Bits [31:25]
        int32_t imm;

        switch (opcode)
        {
        case 0x3:
            imm = (int32_t)(instr & 0xFFF00000) >> 20; // Sign-extend
            execute_l_type(instr, &reg, &memory);
            break;

        case 0x6f:                                    // J-Type
            int32_t imm_20 = (instr >> 31) & 0x1;     // Bit 20 (sign bit)
            int32_t imm_10_1 = (instr >> 21) & 0x3FF; // Bits 10:1
            int32_t imm_11 = (instr >> 20) & 0x1;     // Bit 11
            int32_t imm_19_12 = (instr >> 12) & 0xFF; // Bits 19:12
            imm = (imm_20 << 20) | (imm_19_12 << 12) | (imm_11 << 11) | (imm_10_1 << 1);
            if (imm_20)
            {
                imm |= 0xFFF00000; // Sign-extend to 32 bits
            }
            execute_j_type(rd, rs1, imm, reg, &pc);
            break;

        case 0x13:                                     // I-Type
            imm = (int32_t)(instr & 0xFFF00000) >> 20; // Sign-extend
            execute_i_type(rd, rs1, imm, funct3, funct7, reg);
            break;

        case 0x17: // U-Type
            imm = (instr >> 12) & 0xFFFFF;
            execute_u17_type(rd, imm, &pc, reg);
            break;

        case 0x23: // Store instructions (SB, SH, SW)
            imm = ((instr >> 25) << 5) | ((instr >> 7) & 0x1F);
            execute_s_type(rd, rs1, rs2, funct3, imm, &reg, &memory);
            break;

        case 0x33: // R-Type
            execute_r_type(rd, rs1, rs2, funct3, funct7, reg);
            break;

        case 0x37: // U-Type
            imm = (instr >> 12) & 0xFFFFF;
            execute_u_type(rd, imm, reg);
            break;

        case 0x63:                                         // B-Type
            imm = ((instr >> 7) & 0x1e)                    // bits [1:4]
                  | ((instr >> 20) & 0x7e0)                // bits [5:10]
                  | ((instr << 4) & 0x800)                 // bit [11]
                  | (instr & 0x80000000 ? 0xfffff000 : 0); // Sign-extend

            execute_b_type(rs1, rs2, imm, funct3, &pc, reg);
            continue; // Skip the pc increment for branching

        case 0x67:                                     // JALR
            imm = (int32_t)(instr & 0xFFF00000) >> 20; // Sign-extend
            execute_i0x67_type(rd, rs1, imm, reg, &pc);
            break;

        case 0x73: // ECALL
            execute_ecall(rd, rs1, imm, funct3, funct7, reg);
            break;

        default:
            printf("Opcode %x not implemented\n", opcode);
        }

        // printf("Opcode %x\n", opcode);

        pc += 4; // Increment program counter
        if ((pc >> 2) >= MEMORY_SIZE || memory[pc >> 2] == 0)
            break;
        printRegs(reg, 0);

        printf("\nI = %d PC = %d", i++, pc);
        printf("\n\n");
    }

    // Final state
    printf("Final register states:\n");
    printRegs(reg, 1);
    printf("\n\n");

    return 0;
}
