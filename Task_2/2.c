/*
2.Вывести заданный массив размером N в обратном порядке.
*/

#include <stdio.h>
#define n 5

int main(void){
    int array[n] = {0};

    printf(" Array: ");
    for(int i = 0; i < n; i++){
        array[i] = i + 1;
        printf("%d ", array[i]);
    }

    for(int i = 0; i < n / 2; i++){
        int a = array[n - i - 1];
        array[n - i - 1] = array[i];
        array[i] = a;
    }

    printf("\n~Array: ");
    for(int i = 0; i < n; i++){
        printf("%d ", array[i]);
    }
    printf("\n");

    return 0;
}