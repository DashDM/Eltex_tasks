/*
1.Поменять в целом положительном числе (типа int) значение третьего 
  байта на введенное пользователем число (изначальное число также
  вводится с клавиатуры) через указатель (не применяя битовые
  операции)
*/
#include <stdio.h>

int main(void){
    int value = 0, third_byte = 0;
    unsigned char *ptr = NULL; //беззнаковый [0; 255], просто char [-128; 127] (и тот, и другой тип = 1 байт)
    
    printf("Число: ");
    scanf("%d", &value);
    
    getchar(); // считывает символы из входного потока, включая символы новой строки, и очищает входной поток перед следующим вызовом функции scanf
    
    printf("Значение 3-го байта: ");
    scanf("%d", &third_byte);

    ptr = (unsigned char *)&value;
    
    printf("\n|Исходное число в байтах|\n"); 
    for (int i = 0; i < 4; i++) {
        printf("%d byte: address --> %p | dereferenced_value --> %d\n", i + 1, ptr, *ptr);
        ptr++;
    }
    
    ptr = (unsigned char *)&value; //возвращаемся к началу value (адрес первого байта)
    
    printf("\n|Изменённое число в байтах|\n");
    for (int i = 0; i < 4; i++) {
        if (i == 2) {
            *ptr = third_byte; //в 3 байт первого числа (value) присваиваем значение 1 байта второго числа (third_byte)
          }
        printf("%d byte: address --> %p | dereferenced_value --> %d\n", i + 1, ptr, *ptr);
        ptr++;
    }
    
    return 0;
}
