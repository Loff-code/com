#include <stdio.h>
void printBinary(int n)
{
    for (unsigned mask = 1 << 31; mask > 0; mask >>= 1)
    {
        printf("%d", (n & mask) ? 1 : 0);
    }
    printf("\n");
}
int main()
{
    int a = 0x00200093;                        // Initialize with hexadecimal value
    printf("Hexadecimal: %x,%x\n", a, a >> 2); // Hexadecimal
    printf("Decimal: %d,%d\n", a, a >> 2);     // Decimal
    printBinary(a);
    printBinary(a >> 2);

    return 0;
}
