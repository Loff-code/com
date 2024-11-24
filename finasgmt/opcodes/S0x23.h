

// Content from /mnt/data/STORE_INSTRUCTIONS.h
#ifndef STORE_INSTRUCTIONS_H
#define STORE_INSTRUCTIONS_H

#include <stdint.h>
#include <stdio.h>

// Function prototype
void execute_s_type(int rd, int rs1, int rs2, int funct3, int32_t imm, uint32_t *reg, uint32_t *memory)
{
    // Compute memory address
    int address = reg[rs1] + imm;  // Effective memory address
    int word_index = address / 4;  // Word-aligned memory index
    int byte_offset = address % 4; // Byte offset within the word (0-3)

    // Extract data to store
    uint8_t extracted_byte = reg[rs2] & 0xFF;    // Least significant byte
    uint16_t extracted_half = reg[rs2] & 0xFFFF; // Least significant half-word
    uint32_t extracted_word = reg[rs2];          // Full word

    // Debugging output
    printf("Registers: rs1 (x%d) = 0x%08X, rs2 (x%d) = 0x%08X\n", rs1, reg[rs1], rs2, reg[rs2]);
    printf("Immediate: 0x%08X (%d)\n", imm, imm);
    printf("Address: 0x%X (word_index: %d, byte_offset: %d)\n", address, word_index, byte_offset);
    // printf("Memory before: 0x%08X\n", memory[word_index]);

    // Perform the store operation based on funct3
    switch (funct3)
    {
    case 0x00:                                                       // SB: Store Byte
        memory[word_index] &= ~(0xFF << (byte_offset * 8));          // Clear target byte
        memory[word_index] |= (extracted_byte << (byte_offset * 8)); // Insert byte
        printf("Executed SB: M[%d] = 0x%02X << offset:%d\n", word_index, extracted_byte, byte_offset * 8);
        break;

    case 0x01: // SH: Store Half-word
        if (byte_offset <= 2)
        {                                                                // Fits within a single word
            memory[word_index] &= ~(0xFFFF << (byte_offset * 8));        // Clear target half
            memory[word_index] |= (extracted_half << (byte_offset * 8)); // Insert half-word
        }
        else
        { // Spans two words (unaligned case)
            uint8_t lower_byte = extracted_half & 0xFF;
            uint8_t upper_byte = (extracted_half >> 8) & 0xFF;

            // Store lower byte in current word
            memory[word_index] &= ~(0xFF << (byte_offset * 8));
            memory[word_index] |= (lower_byte << (byte_offset * 8));

            // Store upper byte in the next word
            memory[word_index + 1] &= ~0xFF;
            memory[word_index + 1] |= upper_byte;
        }
        printf("Executed SH: M[%d] = 0x%04X << offset:%d\n", word_index, extracted_half, byte_offset * 8);
        break;

    case 0x02: // SW: Store Word
        if (byte_offset == 0)
        {                                        // Aligned case
            memory[word_index] = extracted_word; // Store full word
        }
        else
        {                                                                      // Unaligned case
            int lower_bits = 32 - byte_offset * 8;                             // Bits fitting in the current word
            uint32_t lower_mask = (1U << lower_bits) - 1;                      // Mask for lower part
            uint32_t upper_mask = ~lower_mask;                                 // Mask for upper part
            uint32_t lower_part = extracted_word & lower_mask;                 // Lower bits
            uint32_t upper_part = (extracted_word & upper_mask) >> lower_bits; // Upper bits

            // Store lower part in the current word
            memory[word_index] &= ~(lower_mask << (byte_offset * 8));
            memory[word_index] |= (lower_part << (byte_offset * 8));

            // Store upper part in the next word
            memory[word_index + 1] &= ~upper_mask;
            memory[word_index + 1] |= upper_part;
        }
        printf("Executed SW: M[%d] = 0x%08X\n", word_index, extracted_word);
        break;

    default:
        printf("Unknown funct3: %d\n", funct3);
        break;
    }

    // Debugging output for memory after modification
    printf("Memory after: 0x%08X\n", memory[word_index]);

    // Print decimal values for each byte
    printf("Memory bytes (decimal): ");
    for (int i = 0; i < 4; i++)
    {
        int8_t byte_value = (memory[word_index] >> (i * 8)) & 0xFF;
        printf("%d ", byte_value);
    }
    printf("\n");
}

#endif // STORE_INSTRUCTIONS_H