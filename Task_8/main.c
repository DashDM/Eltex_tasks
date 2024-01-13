#include <stdio.h>
#include "operations.h"

int main(void) {
    int option = 0, param_x = 0, param_y = 0, result = 0;
    
    printf("[Welcome to caculator 2024]---[option, x, y = result]\n");
    
    do {
        printf("Operation list: | 1.Сложение | 2.Вычитание | 3.Умножение | 4.Деление | 5.Выход |\n");
        printf("Выберите номер --> ");
        scanf("%d", &option);
        
        if (option == 5) {
            return 0;
        }
        
        printf("x = ");
        scanf("%d", &param_x);
        
        printf("y = ");
        scanf("%d", &param_y);
        
        switch (option) {
            case 1:
                result = Addition(param_x, param_y);
                printf("%d + %d = %d\n", param_x, param_y, result);
                break;
            case 2:
                result = Subtraction(param_x, param_y);
                printf("%d - %d = %d\n", param_x, param_y,  result);
                break;
            case 3:
                result = Multiplication(param_x, param_y);
                printf("%d * %d = %d\n", param_x, param_y,  result);
                break;
            case 4:
                result = Division(param_x, param_y);
                printf("%d / %d = %d\n", param_x, param_y,  result);
                break;
            default:
                printf("Wrong number, try again..\n");
        }
    } while (1);
}
