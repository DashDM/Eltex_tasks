#include <stdio.h>
// функция для сравнения строк
int compare_string (char *str) {
    char exit_check[] = "exit";
    int sum = 0;
    
    for (int i = 0; i < 4; i++) {
        if (str[i] == exit_check[i])
            sum++;
    }
    
    if (sum != 4)
        return 0; // если строки не равны
    else
        return 1; // если строки равны
}
