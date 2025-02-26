#include <stdio.h>
 
toggleBitsInRange(int num, int start, int end) {

    int num_size = num;
    int length = 0;
    while (num_size > 0) {
        num_size >>= 1;
        length++;
    }

    printf("size = %d\n", length);

    int mask = 0b0001111;
    int shift = length - 6 + 1;
    int newmask = mask << shift;

    int result = newmask ^ num;

    return result; 
}

int main(void)
{
    int num = 0b010101010;
    int result = toggleBitsInRange(num, 2, 5);
    printf("Результат: %d (%b)\n", result, result);
    
 
    return 0;
}