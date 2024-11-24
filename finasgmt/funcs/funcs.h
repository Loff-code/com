#ifndef FUNCS_H
#define FUNCS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "R0x33.h" // R-Type instructions
#include "B0x63.h" // B-Type instructions
#include "I0x13.h" // I-Type instructions
#include "I0x03.h" // I-Type instructions LOAD
#include "I0x67.h" // I-Type instructions
#include "U0x17.h" // U-Type instructions
#include "U0x37.h" // U-Type instructions
#include "J0x6f.h" // J-Type instructions
#include "S0x23.h" // J-Type instructions

#include "ecall.h" // ECALL instruction

#define MEMORY_SIZE (1024 * 256)
uint32_t memory[MEMORY_SIZE]; // Simulated memory
size_t memory_index = 0;

int pc; // Program counter
int i;
int reg[32]; // Registers

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

void printRegs(int reg[], int flag)
{
    for (int i = 0; i < 32; ++i)
    {
        if (flag == 0)
        {
            if (reg[i] != 0)
            {
                printf("x%d=%d ", i, reg[i]);
            }
        }
        else
        {
            printf("x%d=%d ", i, reg[i]);
        }
    }
}
#endif // FUNCS_H
