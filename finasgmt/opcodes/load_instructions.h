#ifndef LOAD_INSTRUCTIONS_H
#define LOAD_INSTRUCTIONS_H

#include <stdint.h>
#include <stdio.h>

// Function prototype

void execute_load(uint32_t instr, uint32_t *reg, uint32_t *memory)
{
    // Extract immediate (imm[11:0])
    int32_t imm = (instr >> 20) & 0xFFF; // Bits 20-31
    if (imm & 0x800)
    {                      // Check the sign bit (bit 11)
        imm |= 0xFFFFF000; // Sign-extend if necessary
    }

    // Extract registers and funct3
    uint32_t rs1 = (instr >> 15) & 0x1F;    // Base register (bits 19–15)
    uint32_t rd = (instr >> 7) & 0x1F;      // Destination register (bits 11–7)
    uint32_t funct3 = (instr >> 12) & 0x07; // funct3 (bits 14–12)

    // Compute memory address
    int effective_address = reg[rs1] + imm;  // Effective memory address
    int word_index = effective_address / 4;  // Word-aligned memory index
    int byte_offset = effective_address % 4; // Byte offset within the word (0-3)

    // Debugging output
    printf("Instruction: 0x%08X\n", instr);
    printf("Registers: rs1 (x%d) = 0x%08X, rd (x%d)\n", rs1, reg[rs1], rd);
    printf("Immediate: 0x%08X (%d)\n", imm, imm);
    printf("Effective Address: 0x%X (Word Index: %d, Byte Offset: %d)\n", effective_address, word_index, byte_offset);
    printf("Memory at Word Index %d: 0x%08X\n", word_index, memory[word_index]);

    uint8_t extracted_byte = (memory[word_index] >> (byte_offset * 8)) & 0xFF;
    uint16_t extracted_half;
    uint32_t extracted_word;

    // Perform the load operation based on funct3
    switch (funct3)
    {
    case 0x00:                            // LB (Load Byte)
        reg[rd] = (int8_t)extracted_byte; // Sign-extend byte to 32 bits
        printf("LB: Loaded 0x%02X from Memory[%d] (Byte Offset %d)\n", extracted_byte, word_index, byte_offset);
        printf("Result: reg[%d] = 0x%08X\n", rd, reg[rd]);
        break;

    case 0x01: // LH (Load Halfword)
        if (byte_offset <= 2)
        {
            // Halfword fits in one memory word
            extracted_half = (memory[word_index] >> (byte_offset * 8)) & 0xFFFF;
        }
        else
        {
            // Halfword spans two memory words
            uint8_t lower_byte = (memory[word_index] >> (byte_offset * 8)) & 0xFF;
            uint8_t upper_byte = memory[word_index + 1] & 0xFF;
            extracted_half = (upper_byte << 8) | lower_byte;
        }
        reg[rd] = (int16_t)extracted_half; // Sign-extend halfword to 32 bits
        printf("LH: Loaded 0x%04X from Memory[%d] (Byte Offset %d)\n", extracted_half, word_index, byte_offset);
        printf("Result: reg[%d] = 0x%08X\n", rd, reg[rd]);
        break;

    case 0x02: // LW (Load Word)
        if (byte_offset == 0)
        {
            reg[rd] = memory[word_index];
        }
        else
        {
            uint32_t lower_part = memory[word_index] >> (byte_offset * 8);
            uint32_t upper_part = memory[word_index + 1] & ((1U << (byte_offset * 8)) - 1);
            reg[rd] = (upper_part << (32 - byte_offset * 8)) | lower_part;
        }

        printf("LW: Loaded 0x%08X from Memory[%d] (Byte Offset %d)\n", reg[rd], word_index, byte_offset);
        printf("Result: reg[%d] = 0x%08X\n", rd, reg[rd]);
        break;
    case 0x04:                    // LBU (Load Unsigned Byte)
        reg[rd] = extracted_byte; // Zero-extend byte
        printf("LBU: Loaded 0x%02X from Memory[%d] (Byte Offset %d)\n", extracted_byte, word_index, byte_offset);
        break;

    case 0x05: // LHU (Load Unsigned Halfword)
        if (byte_offset <= 2)
        {
            extracted_half = (memory[word_index] >> (byte_offset * 8)) & 0xFFFF;
        }
        else
        {
            uint8_t lower_byte = (memory[word_index] >> (byte_offset * 8)) & 0xFF;
            uint8_t upper_byte = memory[word_index + 1] & 0xFF;
            extracted_half = (upper_byte << 8) | lower_byte;
        }
        reg[rd] = extracted_half; // Zero-extend half-word
        printf("LHU: Loaded 0x%04X from Memory[%d] (Byte Offset %d)\n", extracted_half, word_index, byte_offset);
        break;

    default:
        printf("Unsupported funct3: 0x%X\n", funct3);
        break;
    }
}

#endif // LOAD_INSTRUCTIONS_H
