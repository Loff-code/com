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
        memory_index++;

        if (memory_index >= MEMORY_SIZE)
        {
            fprintf(stderr, "Error: Program exceeds memory size\n");
            break;
        }
    }

    fclose(file);
    printf("Program loaded into memory. Instructions: %zu\n", memory_index);
}

int pc;      // Program counter
int reg[32]; // Registers
int program[] = {
    0xabcdf2b7,
    0xfab28293,
    0x07f00313,
    0x00531023,
};
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

        int opcode = instr & 0x7f; // Extract opcode
        int rd = (instr >> 7) & 0x1f;
        int rs1 = (instr >> 15) & 0x1f;
        int rs2 = (instr >> 20) & 0x1f;
        int imm = (instr >> 20) & 0xfff;
        int funct3 = (instr >> 12) & 0x7;
        int funct7 = (instr >> 25) & 0x7f;
        uint8_t extracted_byte;
        uint32_t effective_address;
        uint32_t word_index;
        uint32_t byte_offset;
        uint16_t extracted_half;
        uint16_t extracted_word;
        if (imm & 0x800)
            imm |= 0xfffff000; // Sign-extend immediate

        switch (opcode)
        {
        case 0x3: //
            execute_load(instr, &reg, &memory);
            break;
        case 0x6f:                                        // J-Type
            imm = ((instr >> 12) & 0xff) << 12;           // bits [19:12]
            imm |= ((instr >> 20) & 1) << 11;             // bit [11]
            imm |= ((instr >> 21) & 0x3ff) << 1;          // bits [10:1]
            imm |= (instr & 0x80000000) ? 0xfffff000 : 0; // sign-extend bit [20]

            execute_j_type(rd, rs1, imm, funct3, funct7, reg, &pc);

            break;
        case 0x13: // I-Type
            execute_i_type(rd, rs1, imm, funct3, funct7, reg);
            break;
        case 0x17: // U-Type
            execute_u17_type(rd, instr >> 12, funct3, &pc, reg);
            break;
        case 0x23: // Store instructions (SB, SH, SW)
            execute_store(instr, &reg, &memory);

            break;

        case 0x33: // R-Type
            execute_r_type(rd, rs1, rs2, funct3, funct7, reg);
            break;
        case 0x37: // U-Type
            execute_u_type(rd, rs1, instr >> 12, funct3, reg);
            break;
        case 0x63: // B-Type
            execute_b_type(rs1, rs2, instr, funct3, &pc, reg);
            continue; // Skip the pc increment for branching
        case 0x67:    // JALR
            reg[rd] = pc + 4;
            printf("Executed JALR: jump to initial and x1 = current + 4 || x%d = %d\n", rd, reg[rd]);
            pc = imm;
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

        if (i > 100 * 100 + 20)
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
