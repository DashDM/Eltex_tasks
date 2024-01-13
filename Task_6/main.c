#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "service.h"

int main(void) {
    struct abonent *List = malloc(0);
    int count = 0;
    bool cycle = true;
    
    printf("1)Добавить абонента\n2)Удалить абонента\n3)Поиск абонентов по имени\n4)Вывод всех записей\n5)Выход\n");
    do {
        short option = 0;
        printf("to do --> ");
        scanf("%hd", &option);
        getchar();
        
        switch(option) {
            case 1:
                count++;
                List = realloc(List, count * sizeof(struct abonent));
                add(List, count);
                break;
                
            case 2:
                int del_index = -1;
                printf("Введите порядковый номер абонента, которого хотите удалить: ");
                scanf("%d", &del_index);
                getchar();
                del(List, &count, del_index);
                List = realloc(List, count * sizeof(struct abonent));
                break;
                
            case 3:
                char find_name[field_size] = { 0 };
                printf("Введите имя, которое хотите найти: ");
                scanf("%s", find_name);
                getchar();
                find(List, count, find_name);
                break;
                
            case 4:
                out(List, count);
                break;
                
            case 5:
                free(List);
                cycle = false;
                break;

            default:
                printf("Опция не найдена..\n");
        }
    } while (cycle);
    
    return 0;
}

