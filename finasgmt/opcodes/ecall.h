#ifndef ecall_h
#define ecall_h

#include <stdio.h>
#include <stdlib.h>
void execute_ecall(int rd, int rs1, int imm, int funct3, int funct7, int reg[])
{
    // Check the value of a7 to determine the type of ECALL
    if (reg[17] == 10) // Register a7 is x17
    {
        printf("Program exited successfully via ECALL\n");
        // Print final register states
        printf("Final register states:\n");
        for (int i = 0; i < 32; ++i)
        {

            printf("x%d=%d ", i, reg[i]);
        }
        printf("\n");
        exit(0); // Stop the program
    }
    else
    {
        printf("Unhandled ECALL with a7 = %d\n", reg[17]);
        exit(0); // Stop the program
    }
}

#endif // ECALL
