#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define NUM_REGISTERS 32
#define BYTES_PER_REGISTER 4

void print_binary(int32_t value)
{
    for (int i = 31; i >= 0; i--)
    {
        putchar((value & (1U << i)) ? '1' : '0');
    }
}

void read_registers_from_file(const char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (!file)
    {
        perror("Error opening file");
        return;
    }

    int32_t registers[NUM_REGISTERS];
    size_t count = fread(registers, sizeof(int32_t), NUM_REGISTERS, file);

    if (count != NUM_REGISTERS)
    {
        fprintf(stderr, "Warning: Expected %d registers, but read %zu\n", NUM_REGISTERS, count);
    }

    printf("Register values:\n");
    for (size_t i = 0; i < count; i++)
    {
        if (registers[i] != 0)
        {
            printf("x%zu=", i);
            // print_binary(registers[i]);               // Print the register value in binary
            printf("%x ", registers[i]); // Print the decimal equivalent
        }
    }

    fclose(file);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <file_path>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    read_registers_from_file(filename);
    printf("\n"); // Print the decimal equivalent

    return 0;
}
