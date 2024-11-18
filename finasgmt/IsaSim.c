#include <stdio.h>
#include "variables/variables.h"
#include "funcs/aidsFunc.h"
int pc;
int reg[4];
int progr[] = {
    0x00200093, // addi x1 x0 2
    0x00300113, // addi x2 x0 3
    0x002081b3  // add x3 x1 x2
};

int main(int argc, char const *argv[])
{
    pc = 0;
    printf("Hello RISC-V World!\n");
    // printf("%i", aids());
    while (1)
    {

        int instr = progr[pc >> 2];
        int opcode = instr & 0x7f;
        int rd = (instr >> 7) & 0x01f;
        int rs1 = (instr >> 15) & 0x01f;
        int imm = (instr >> 20);

        switch (opcode)
        {
        case 51:
            printf("opcode: %i is here!\n", opcode);
            break;
        case 0x13:
            reg[rd] = reg[rs1] + imm;
            break;
        default:
            // System.out.println("Opcode " + opcode + " not yet implemented");
            printf("Opcode %i not yet implemented", opcode);
            break;
        }

        pc += 4; // One instruction is four bytes
        if ((pc >> 2) >= sizeof(progr) / sizeof(progr[0]))
        {
            break;
        }
        for (int i = 0; i < sizeof(reg) / sizeof(reg[0]); ++i)
        {
            printf("%d ", reg[i]);
        }
        printf("\n");
    }

    printf("Program exit\n");
    while (1)
    {
        // printf("hello");
        
    }
    
    return 0;
}
