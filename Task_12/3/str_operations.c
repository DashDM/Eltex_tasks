#include "operations_third.h"
// ввод строки
void my_bash_input(char *string) {
    while (1) {
        printf("my_bash$ ");
        fgets(string, MAX_INPUT_LENGTH, stdin);
        
        if (string[0] != '\n')
            break;
    }
    
    // заменяем '\n' на '\0'
    del_newline_character(string);
}
// функция для удаления символа новой строки '\n', чтобы путь не содержал лишний символ
void del_newline_character(char *string) {
    int length = 0;
    while (string[length] != '\0') {
        if (string[length] == '\n') {
            string[length] = '\0';
            break;
        }
        length++;
    }
}
// разделение строки на команду и атрибут
void partition_command(char *string, int separator, char *left_command[], char *right_command[]) {
    int ch_count = 0; // символьный счётчик (для чтения введённой строки)
    
    // если нет разделителя
    if (right_command == NULL) {
        // для каждого аргумента:
        for (int j = 0; j < MAX_ARGS; j++) {
            int t = 0; // символьный счётчик (для записи в массив)
            // исключаем пробелы
            while (string[ch_count] == ' ')
                ch_count++;
            // поэлементно считываем строку в массив указателей на строки
            while((string[ch_count] != ' ') && (string[ch_count] != '\0') && (string[ch_count] != '|')) {
                left_command[j][t] = string[ch_count];
                t++;
                ch_count++;
            }
            // зануляем последний элемент каждой строки
            left_command[j][t] = '\0';
            // если дошли до конца строки, выходим
            if (string[ch_count] == '\0')
                break;
        }
    } else {
        // для каждой команды:
        for (int i = 1; i < separator + 1; i++) {
            // для каждого аргумента:
            for (int j = 0; j < MAX_ARGS; j++) {
                int t = 0; // символьный счётчик (для записи в массив)
                // для первой команды:
                if (i == 1) {
                    // избавляемся от пробелов (ищем команду/атрибут)
                    while (string[ch_count] == ' ')
                        ch_count++;
                    // поэлементно заполняем массив
                    while((string[ch_count] != ' ') && (string[ch_count] != '\0') && (string[ch_count] != '|')) {
                        left_command[j][t] = string[ch_count];
                        t++;
                        ch_count++;
                    }
                    // зануляем последний элемент каждой строки
                    left_command[j][t] = '\0';
                }
                // по аналогии для второй команды, но с учётом разделителя:
                if (i == 2) {
                    while ((string[ch_count] == ' ') || (string[ch_count] == '|'))
                        ch_count++;
                    
                    while((string[ch_count] != ' ') && (string[ch_count] != '\0') && (string[ch_count] != '|')) {
                        right_command[j][t] = string[ch_count];
                        t++;
                        ch_count++;
                    }
                    right_command[j][t] = '\0';
                }
            }
        }
    }
}

