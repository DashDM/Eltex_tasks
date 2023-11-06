/*
3)Найти кол-во единиц в двоичном представлении целого положительного числа
(число вводится с клавиатуры).
*/

#include <stdio.h>

int main(void)
{
    int number = 0, sum = 0;

    do{
        printf("Enter n > 0\nnumber = ");
        scanf("%d", &number);
    }while(number < 0);

    printf("dec_to_bin: ");
    for(int i = sizeof(number) * 8 - 1; i >= 0; i--){
        int bit = (number >> i) & 1;
        if(bit){
            sum++;
            printf("1");
        }
        else
            printf("0");
    }
    printf("\nsum: %d\n", sum);

    return 0;
}