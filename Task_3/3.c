/*
3.Напишите программу, в которой определите массив типа int (размер
  массива 10 элементов от 1 до 10). Используйте указатель для вывода
  элементов массива на консоль (применять можно только арифметику
  указателей, а не индексы).
*/
#include <stdio.h>

int main(void){
    int array[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    int *ptr = &array[0];
    
    printf("Array: ");
    for (int i = 0; i < 10; i++) {
        printf("%d ", *ptr);
        ptr++;
    }
    printf("\n");
    
    return 0;
}
