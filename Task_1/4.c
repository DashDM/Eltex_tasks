/*
4)Поменять в целом положительном числе (типа int) значение третьего бита на
введённое пользователем число (изначально число также вводится с клавы).
*/

#include <stdio.h>

int main(void)
{
    int number = 0, value = 0, new_num = 0;

    do{
        printf("Enter n > 0\nnumber = ");
        scanf("%d", &number);
    }while(number < 0);

    do{
        printf("Enter val = 0 or val = 1\nvalue = ");
        scanf("%d", &value);
        if((value == 0) || (value == 1))
            break;
    }while(1);

    printf("dec_to_bin: ");
    for(int i = sizeof(number) * 8 - 1; i >= 0; i--){
        int bit = (number >> i) & 1;
        if(bit){
            printf("1");
        }
        else
            printf("0");
    }

    new_num = (number & ~(1 << 2)) | (value << 2);
    
    printf("\nnew_to_bin: ");
    for(int i = sizeof(new_num) * 8 - 1; i >= 0; i--){
        int bit = (new_num >> i) & 1;
        if(bit){
            printf("1");
        }
        else
            printf("0");
    }

    printf("\nnew_num = %d\n", new_num);

    return 0;
}