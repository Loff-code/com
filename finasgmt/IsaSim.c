#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "R0x33.h"              // R-Type instructions
#include "B0x63.h"              // B-Type instructions
#include "I0x13.h"              // I-Type instructions
#include "U0x17.h"              // U-Type instructions
#include "U0x37.h"              // U-Type instructions
#include "J0x6f.h"              // J-Type instructions
#include "STORE_INSTRUCTIONS.h" // J-Type instructions
#include "load_instructions.h"
#include "ecall.h"               // ECALL instruction
#define MEMORY_SIZE (1024 * 256) // Memory in words
// Adjust as needed
uint32_t memory[MEMORY_SIZE]; // Simulated memory
size_t memory_index = 0;
void load_bin_file(const char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (!file)
    {
        perror("Error opening file");
        return;
    }

    uint8_t memory4piece[4]; // Temporary buffer for 4 bytes

    while (fread(memory4piece, sizeof(uint8_t), 4, file) == 4)
    {
        // Combine the 4 bytes into a 32-bit word
        uint32_t combined_instruction = 0;
        for (size_t i = 0; i < 4; i++)
        {
            combined_instruction |= (memory4piece[i] << (i * 8)); // Combine bytes
        }

        // Store the instruction in the simulator memory
        memory[memory_index] = combined_instruction;

        // Print machine code in binary and hex
        printf("Instruction %zu: 0x%08X (", memory_index, combined_instruction);
        for (int i = 31; i >= 0; i--)
        {
            printf("%c", (combined_instruction & (1U << i)) ? '1' : '0');
            if (i % 4 == 0 && i != 0)
                printf(" ");
        }
        printf(")\n");

        memory_index++;

        if (memory_index >= MEMORY_SIZE)
        {
            fprintf(stderr, "Error: Program exceeds memory size\n");
            break;
        }
    }

    fclose(file);
    printf("Program loaded into memory. Total instructions: %zu\n", memory_index);
}

int pc;      // Program counter
int reg[32]; // Registers
// int program[] = {
//     0xabcdf2b7,
//     0xfab28293,
//     0x07f00313,
//     0x00531023,
// };
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <program.bin>\n", argv[0]);
        return 1;
    }

    pc = 0; // Initialize program counter

    // Load the binary program into memory
    load_bin_file(argv[1]);

    printf("Hello RISC-V World!\n");
    int i = 0;
    // for (size_t i = 0; i < 11; i++)
    while (1)
    {

        // int instr = program[pc >> 2];
        int instr = memory[pc >> 2]; // Fetch instruction
        printf("Instruction: 0x%08X\n", instr);

        // uint32_t opcode = instr & 0x7f; // Extract opcode
        // int rd = (instr >> 7) & 0x1f;
        // int rs1 = (instr >> 15) & 0x1f;
        // int rs2 = (instr >> 20) & 0x1f;
        // int imm = (instr >> 20) & 0xfff;
        // int funct3 = (instr >> 12) & 0x7;
        // int funct7 = (instr >> 25) & 0x7f;
        uint8_t opcode = instr & 0x7F;         // Bits [6:0]
        uint8_t rd = (instr >> 7) & 0x1F;      // Bits [11:7]
        uint8_t funct3 = (instr >> 12) & 0x7;  // Bits [14:12]
        uint8_t rs1 = (instr >> 15) & 0x1F;    // Bits [19:15]
        uint8_t rs2 = (instr >> 20) & 0x1F;    // Bits [24:20]
        uint8_t funct7 = (instr >> 25) & 0x7F; // Bits [31:25]
        int32_t imm;

        uint8_t extracted_byte;
        uint16_t extracted_half;
        uint32_t extracted_word;

        uint32_t effective_address;
        uint32_t word_index;
        uint32_t byte_offset;
        // if (imm & 0x800)
        //     imm |= 0xfffff000; // Sign-extend immediate

        switch (opcode)
        {
        case 0x3:                                      //
            imm = (int32_t)(instr & 0xFFF00000) >> 20; // Sign-extend

            execute_load(instr, &reg, &memory);
            break;
        case 0x6f: // J-Type
                   // imm = ((instr >> 31) << 20) | (((instr >> 12) & 0xFF) << 12) |
                   //       (((instr >> 20) & 0x1) << 11) | ((instr >> 21) & 0x7FF);

            int32_t imm_20 = (instr >> 31) & 0x1;     // Bit 20 (sign bit)
            int32_t imm_10_1 = (instr >> 21) & 0x3FF; // Bits 10:1
            int32_t imm_11 = (instr >> 20) & 0x1;     // Bit 11
            int32_t imm_19_12 = (instr >> 12) & 0xFF; // Bits 19:12

            // Combine the fields into a single immediate value
            imm = (imm_20 << 20) | (imm_19_12 << 12) | (imm_11 << 11) | (imm_10_1 << 1);

            // Sign-extend the immediate if the sign bit (imm_20) is set
            if (imm_20)
            {
                imm |= 0xFFF00000; // Sign-extend to 32 bits
            }
            execute_j_type(rd, rs1, imm, funct3, funct7, reg, &pc);

            break;
        case 0x13:                                     // I-Type
            imm = (int32_t)(instr & 0xFFF00000) >> 20; // Sign-extend

            execute_i_type(rd, rs1, imm, funct3, funct7, reg);
            break;
        case 0x17: // U-Type
            imm = instr & 0xFFFFF000;
            execute_u17_type(rd, instr >> 12, funct3, &pc, reg);
            break;
        case 0x23: // Store instructions (SB, SH, SW)
            imm = ((instr >> 25) << 5) | ((instr >> 7) & 0x1F);

            execute_store(instr, &reg, &memory);

            break;

        case 0x33: // R-Type
            execute_r_type(rd, rs1, rs2, funct3, funct7, reg);
            break;
        case 0x37: // U-Type
            imm = instr & 0xFFFFF000;
            execute_u_type(rd, rs1, instr >> 12, funct3, reg);
            break;
        case 0x63: // B-Type
            imm = ((instr >> 31) << 12) | (((instr >> 7) & 0x1) << 11) |
                  (((instr >> 25) & 0x3F) << 5) | ((instr >> 8) & 0xF);

            execute_b_type(rs1, rs2, instr, funct3, &pc, reg);
            continue;                                  // Skip the pc increment for branching
        case 0x67:                                     // JALR
            imm = (int32_t)(instr & 0xFFF00000) >> 20; // Sign-extend
            if (rd != 0)
            {
                reg[rd] = pc + 4;
            }

            printf("Executed JALR: jump to initial and x1 = current + 4 || x%d = %d\n", rd, reg[rd]);
            pc = imm + reg[rs1] - 4;
            break;
        case 0x73: // ECALL
            execute_ecall(rd, rs1, imm, funct3, funct7, reg);
            break;

        default:
            printf("Opcode %x not implemented\n", opcode);
        }
        printf("Opcode %x\n", opcode);
        pc += 4; // Increment program counter
        if ((pc >> 2) >= MEMORY_SIZE || memory[pc >> 2] == 0)
            break;

        // Print register states after each instruction
        for (int i = 0; i < sizeof(reg) / sizeof(reg[0]); ++i)
        {
            if (reg[i] != 0)
            {
                printf("x%d=%d ", i, reg[i]);
            }
        }

        if (i > 10050)
        {
            printf("\n");
            return 0;
        }

        // if (pc > 324)
        // {
        //     printf("\n");
        //     return 0;
        // }
        printf("\nI = %d PC = %d", i++, pc);
        printf("\n\n");
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
    printf("\n\n");

    return 0;
}
