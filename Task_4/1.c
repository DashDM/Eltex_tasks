#include <stdio.h>
#define n 5 // длина массива структур
#define field_size 10 // длина полей в структуре

struct abonent {
    char name[field_size];
    char second_name[field_size];
    char tel[field_size];
};

int main(void) {
    struct abonent list[n] = { 0 }; // инициализируем массив структур '\0'
    short cycle = 1; // условие для цикла
    
    printf("1)Добавить абонента\n2)Удалить абонента\n3)Поиск абонентов по имени\n4)Вывод всех записей\n5)Выход\n");
    do {
        int option = 0; // выбор опции (от 1 до 5). Хотел с помощью enum сделать, но тогда доп. switch нужен, либо много if(ов) - это выглядит, как лишнее
        printf("to do --> ");
        scanf("%d", &option);
        getchar();
        
        switch(option) {
            case 1:
                struct abonent *by_empty = &list[0]; // указатель на адрес первой структуры
                short add_check = 1; // если остаётся 1, то массив полон
                
                for (int i = 0; i < n; i++) { // идём по массиву структур
                    if (by_empty->name[0] == '\0') { // если нашли поле, с первым символом в имени равному символу конца строки, то ячейку массива будем считать пустой
                        printf("Для добавление в справочник введите: Имя (пробел) Фамилия (пробел) Телефон\n");
                        scanf("%s %s %s", list[i].name, list[i].second_name, list[i].tel); // заполняем структуру
                        getchar();
                        add_check = 0;
                        break;
                    }
                    else {
                        by_empty++; // перемещаем указатель по адресу + 1*sizeof(struct abonent)
                    }
                }
                
                if (add_check) {
                    printf("Опа, перебор. Справочник заполнен.\n");
                }
                break;
                
            case 2:
                int del = -1; // -1, чтобы не удалить 0-ую структуру
                
                printf("Введите порядковый номер абонента, которого хотите удалить: ");
                scanf("%d", &del);
                getchar();
                
                if((del >= 0) && (del <= n)) {
                    del--;
                    for(int i = 0; i < field_size; i++) { // обнуляем поля по-элементно
                        list[del].name[i] = '\0';
                        list[del].second_name[i] = '\0';
                        list[del].tel[i] = '\0';
                    }
                    printf("Удаление прошло успешно.\n");
                }
                else {
                    printf("Ошибка. Такого абонента не существует.\n");
                }
                break;
                
            case 3:
                char find_name[field_size] = { 0 };
                int find_check = 0; // 0, если никого не нашли
                
                printf("Введите имя, которое хотите найти: ");
                scanf("%[^\n]", find_name);
                getchar();
                
                for (int i = 0; i < n; i++) { // идём по структурам
                    int name_check = 0; // чтобы считать то, сколько символов совпало между введённым именем и рассматриваемым
                    for (int j = 0; j < field_size; j++) { // идём по именам
                        if (list[i].name[j] != find_name[j]) { // посимвольное сравнение
                            break; // если не совпало, то выходим
                        }
                        else {
                            name_check++; // иначе число совпадений увеличиваем на 1
                        }
                    }
                    if (name_check == field_size) { // если все символы совпали, то абонент найден.
                        printf("%d) %s %s %s\n", i + 1, list[i].name, list[i].second_name, list[i].tel);
                        find_check++;
                    } // Далее переходим к следующей структуре в массиве и сравниваем посимвольно следующее имя
                }
                
                if (find_check == 0) {
                    printf("Абонент(ы) не найден(ы).\n");
                }
                break;
                
            case 4:
                printf("======[Весь список]======\n");
                
                for(int i = 0; i < n; i++) {
                    printf("%d) %s %s %s\n", i + 1, list[i].name, list[i].second_name, list[i].tel);
                }
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
