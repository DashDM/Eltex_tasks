#include <stdio.h>
// функция для удаления \n из массива
void del_newline_character (char *str) {

    int length = 0;
    while (str[length] != '\0') {
        if (str[length] == '\n') {
            str[length] = '\0';
        }
        length++;
    }
    str[length] = 0;
}
