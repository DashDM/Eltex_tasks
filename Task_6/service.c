#include <stdio.h>
#include <string.h>
#define field_size 10

struct abonent {
    char name[field_size];
    char second_name[field_size];
    char tel[field_size];
};

void add(struct abonent *List, int count) {
    printf("Для добавление в справочник введите: Имя (пробел) Фамилия (пробел) Телефон\n");
    scanf("%s %s %s", List[count - 1].name, List[count - 1].second_name, List[count - 1].tel);
    getchar();
}

void del(struct abonent *List, int *count, int del_index) {
    if((del_index >= 0) && (del_index <= *count)) {
        del_index --;
        for (int i = del_index; i < *count; i++) {
            if (i != *count - 1) {
                for( int j = 0; j< field_size; j++) {
                    List[i].name[j] = List[i + 1].name[j];
                    List[i].second_name[j] = List[i + 1].second_name[j];
                    List[i].tel[j] = List[i + 1].tel[j];
                }
            }
            else {
                break;
            }
        }
        *count = *count - 1; 
        printf("Удаление прошло успешно.\n");
    }
    else {
        printf("Ошибка. Такого абонента не существует.\n");
    }
}

void find(struct abonent *List, int count, char *find_name) {
    int find_check = 0; // 0, если никого не нашли
    for (int i = 0; i < count; i++) { // идём по структурам
        if (strcmp(List[i].name, find_name) == 0) {
            printf("%d) %s %s %s\n", i + 1, List[i].name, List[i].second_name, List[i].tel);
            find_check++;
        }
    }
    if (find_check == 0) {
        printf("Абонент(ы) не найден(ы).\n");
    }
}

void out(struct abonent *List, int count) {
    printf("======[Весь список]======\n");
    for(int i = 0; i < count; i++) {
        printf("%d) %s %s %s\n", i + 1, List[i].name, List[i].second_name, List[i].tel);
    }
}
