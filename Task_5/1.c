#include <stdio.h>
#include <string.h>
#define n 5 // длина массива структур
#define field_size 10 // длина полей в структуре

struct abonent {
    char name[field_size];
    char second_name[field_size];
    char tel[field_size];
};

void add(struct abonent*, short);
void del(struct abonent*, int);
void find(struct abonent*, char*);
void out(struct abonent*, short);

int main(void) {
    struct abonent list[n] = { 0 }; // инициализируем массив структур '\0'
    short cycle = 1; // условие для цикла DO WHILE
    
    printf("1)Добавить абонента\n2)Удалить абонента\n3)Поиск абонентов по имени\n4)Вывод всех записей\n5)Выход\n");
    do {
        int option = 0; // выбор опции (от 1 до 5).
        printf("to do --> ");
        scanf("%d", &option);
        getchar();
        
        switch(option) {
            case 1:
                short add_check = 1; // если остаётся 1, то массив полон
                add(list, add_check);
                break;
                
            case 2:
                int del_index = -1; // -1, чтобы не удалить 0-ую структуру
                printf("Введите порядковый номер абонента, которого хотите удалить: ");
                scanf("%d", &del_index);
                getchar();
                del(list, del_index);
                break;
                
            case 3:
                char find_name[field_size] = { 0 };
                printf("Введите имя, которое хотите найти: ");
                scanf("%s", find_name);
                getchar();
                find(list, find_name);
                break;
                
            case 4:
                out(list, n);
                break;
                
            case 5:
                cycle = 0;
                break;

            default:
                printf("Опция не найдена..\n");
        }
    } while (cycle);
    
    return 0;
}

void add(struct abonent *list, short add_check) {
    for (int i = 0; i < n; i++) {
        if (list[i].name[0] == '\0') {
            printf("Для добавление в справочник введите: Имя (пробел) Фамилия (пробел) Телефон\n");
            scanf("%s %s %s", list[i].name, list[i].second_name, list[i].tel);
            getchar();
            add_check = 0;
            break;
        }
    }
    
    if (add_check) {
        printf("Опа, перебор. Справочник заполнен.\n");
    }
}

void del(struct abonent *list, int del_index) {
    if((del_index >= 0) && (del_index < n)) {
        del_index --;
        for(int i = 0; i < field_size; i++) { // обнуляем поля по-элементно
            list[del_index].name[i] = '\0';
            list[del_index].second_name[i] = '\0';
            list[del_index].tel[i] = '\0';
        }
        printf("Удаление прошло успешно.\n");
    }
    else {
        printf("Ошибка. Такого абонента не существует.\n");
    }
}

void find(struct abonent *list, char *find_name) {
    int find_check = 0; // 0, если никого не нашли
    for (int i = 0; i < n; i++) { // идём по структурам
        if (strcmp(list[i].name, find_name) == 0) {
            printf("%d) %s %s %s\n", i + 1, list[i].name, list[i].second_name, list[i].tel);
            find_check++;
        }
    }
    if (find_check == 0) {
        printf("Абонент(ы) не найден(ы).\n");
    }
}

void out(struct abonent *list, short size) {
    printf("======[Весь список]======\n");
    for(int i = 0; i < size; i++) {
        printf("%d) %s %s %s\n", i + 1, list[i].name, list[i].second_name, list[i].tel);
    }
}

