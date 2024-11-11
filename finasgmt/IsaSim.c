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
    printf("Hello RISC-V World!");
    printf("%i", aids());

    return 0;
}
