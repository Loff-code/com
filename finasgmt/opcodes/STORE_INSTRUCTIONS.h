#ifndef STORE_INSTRUCTIONS_H
#define STORE_INSTRUCTIONS_H

#include <stdint.h>
#include <stdio.h>

#define MEMORY_SIZE 1024 // Memory has 1024 words (4KB)

// Function prototype
void execute_store(uint32_t instr, uint32_t *reg, uint32_t *memory)
{
    // Extract lower and upper parts of the immediate
    int32_t imm_lower = (instr >> 7) & 0x1F;    // imm[4:0] (bits 7–11)
    int32_t imm_upper = (instr >> 25) & 0x7F;   // imm[11:5] (bits 25–31)
    int32_t imm = (imm_upper << 5) | imm_lower; // Combine into a 12-bit value

    // Sign-extend the 12-bit immediate to 32 bits
    if (imm & 0x800)
    {
        imm |= 0xFFFFF000;
    }

    // Extract registers and funct3
    uint32_t rs1 = (instr >> 15) & 0x1F;    // Base register (bits 19–15)
    uint32_t rs2 = (instr >> 20) & 0x1F;    // Source register (bits 24–20)
    uint32_t funct3 = (instr >> 12) & 0x07; // funct3 (bits 14–12)

    // Compute memory address
    int address = reg[rs1] + imm;  // Effective memory address
    int word_index = address / 4;  // Word-aligned memory index
    int byte_offset = address % 4; // Byte offset within the word (0-3)

    // Validate memory bounds
    if (word_index < 0 || word_index >= MEMORY_SIZE)
    {
        printf("Error: Invalid memory access at address 0x%X (word index: %d)\n", address, word_index);
        return;
    }

    // Extract data to store
    uint8_t extracted_byte = reg[rs2] & 0xFF;    // Least significant byte
    uint16_t extracted_half = reg[rs2] & 0xFFFF; // Least significant half-word
    uint32_t extracted_word = reg[rs2];          // Full word

    // Debugging output
    printf("Instruction: 0x%08X\n", instr);
    printf("Registers: rs1 (x%d) = 0x%08X, rs2 (x%d) = 0x%08X\n", rs1, reg[rs1], rs2, reg[rs2]);
    printf("Immediate: 0x%08X (%d)\n", imm, imm);
    printf("Effective Address: 0x%X (Word Index: %d, Byte Offset: %d)\n", address, word_index, byte_offset);
    printf("Memory before: 0x%08X\n", memory[word_index]);

    // Perform the store operation based on funct3
    switch (funct3)
    {
    case 0x00:                                                       // SB: Store Byte
        memory[word_index] &= ~(0xFF << (byte_offset * 8));          // Clear target byte
        memory[word_index] |= (extracted_byte << (byte_offset * 8)); // Insert byte
        printf("Executed SB: Memory[%d] updated with byte 0x%02X\n", word_index, extracted_byte);
        break;

    case 0x01: // SH: Store Half-word
        if (byte_offset <= 2)
        {
            memory[word_index] &= ~(0xFFFF << (byte_offset * 8));        // Clear target half
            memory[word_index] |= (extracted_half << (byte_offset * 8)); // Insert half-word
        }
        else
        {
            printf("Error: Unaligned half-word store at address 0x%X\n", address);
            return;
        }
        printf("Executed SH: Memory[%d] updated with half-word 0x%04X\n", word_index, extracted_half);
        break;

    case 0x02: // SW: Store Word
        if (byte_offset == 0)
        {
            memory[word_index] = extracted_word; // Store full word
        }
        else
        {
            printf("Error: Unaligned word store at address 0x%X\n", address);
            return;
        }
        printf("Executed SW: Memory[%d] updated with word 0x%08X\n", word_index, extracted_word);
        break;

    default:
        printf("Error: Unknown funct3 value (%d)\n", funct3);
        return;
    }

    // Debugging output for memory after modification
    printf("Memory after: 0x%08X\n", memory[word_index]);
}

#endif // STORE_INSTRUCTIONS_H
