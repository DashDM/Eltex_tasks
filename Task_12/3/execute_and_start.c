#include "operations_third.h"
// анализ и запуск выполнения введённой команды
void execute_command(char *string) {
    // находим длину введённой строки и проверяем на наличие разделителя '|'
    int separator = 1, str_length = 0;
    while (string[str_length] != '\0') {
        if (string[str_length] == '|')
            separator = str_length;
        str_length++;
    }
    // сравниваем поэлементно введённую строку на наличие запроса 'exit'
    if (str_length == 4) {
        char exit_check[] = "exit";
        int check = 0;
        for(int i = 0; i < str_length; i++) {
            if (string[i] == exit_check[i])
                check++;
        }
        
        if (check == 4)
            exit(1);
    }
    // разбиваем строку на команду(ы) и утрибут(ы)
    // если есть разделитель:
    if (separator != 1) {
        // объявляем символьные массивы указателей на строки (т.е. указатель -> [строка1], [строка2], ... [MAX_ARGS])
        char *left_command[MAX_ARGS], *right_command[MAX_ARGS];
        // для каждой строки массива указателей динамически выделяем память (не самый эффективный вариант, т.к. было бы лучше подсчитать наиб. кол-во символов для каждой команды)
        for(int i = 0; i < MAX_ARGS; i++) {
            left_command[i] = malloc(sizeof(char) * MAX_CMD_SIZE);
            right_command[i] = malloc(sizeof(char) * MAX_CMD_SIZE);
        }
        // вызов функции для разделения команд(ы) от атрибутов(а)
        partition_command(string, separator, left_command, right_command);
        // запуск программы (там процессы и неименованные каналы)
        start_command(separator, left_command, right_command);
        // высвобождаем память, чтобы ОС не парилась
        for(int i = 0; i < MAX_ARGS; i++) {
            free(left_command[i]);
        }
        for(int i = 0; i < MAX_ARGS; i++) {
            free(right_command[i]);
        }
    } else {
        // по аналогии для одной команды (ввод без разделителя)
        char *command[MAX_ARGS];
        for(int i = 0; i < MAX_ARGS; i++)
            command[i] = malloc(sizeof(char) * MAX_CMD_SIZE);
        
        partition_command(string, separator, command, NULL);
        start_command(separator, command, NULL);
        
        for(int i = 0; i < MAX_ARGS; i++) {
            free(command[i]);
        }
    }
    
}
// запуск команд(ы)
void start_command(int separator, char *left_command[], char *right_command[]) {
    // инициализируем массив указателей-аргументов для функции execvp() для первой команды;
    char *args[3] = {left_command[0], left_command[1], NULL};
    if (left_command[1][0] == '\0' || left_command[1][0] == '\n')
        args[1] = NULL;
    // если команда одно:
    if (separator == 1) { 
        // (здесь необходимо использовать дочерний процесс для бесконечного выполнения программы, т.к. execvp() при успешном выполнение всё завершит)
        int status; // объявляем переменную-статус
        pid_t pid = fork();// создаём процесс
        
        if (pid < 0) {
            perror("Ошибка при создании дочернего процесса №1\n");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // дочерний процесс:
            if (execvp(left_command[0], args) < 0) {
                perror("EXECVP не удалось");
                exit(EXIT_FAILURE);
            }
        } else {
            // родитель:
            waitpid(pid, &status, 0); // явное ожидание завершения дочернего процесса
        }
    } else {
        // для двух команд:
        int status1, status2;
        pid_t pid1, pid2;
        int fd[2]; // массив для дескрипторов
        // создаём неименованный канал и делаем проверку
        if (pipe(fd) < 0) {
            perror("Ошибка при создании канала!\n");
            exit(EXIT_FAILURE);
        }
        
        pid1 = fork(); // дочерний процесс для первой команды
        
        if (pid1 < 0) {
            perror("Ошибка при создании дочернего процесса №1\n");
            exit(EXIT_FAILURE);
        } else if(pid1 == 0) {
            // дочерний процесс:
            dup2(fd[1], 1); // копируем адрес дескриптора для записи в канал в стандартный дескриптор ввода
            close(fd[0]); // закрываем дескриптор для чтения
            // выполняем первую команду
            if (execvp(left_command[0], args) < 0) {
                perror("EXECVP не удалось");
                exit(EXIT_FAILURE);
            }
        } else {
            // родительский процесс:
            waitpid(pid1, &status1, 0);
            close(fd[1]); // закрываем дескриптор для записи в канал
            
            pid2 = fork(); // дочерний процесс для второй команды
            
            if (pid2 < 0) {
                perror("Ошибка при создании дочернего процесса №2\n");
            } else if (pid2 == 0) {
                // дочерний процесс:
                dup2(fd[0], 0); // копируем адрес дескриптора для чтения в канал в стандартный дескриптор вывода
                close(fd[1]); // закрываем дескриптор для записи
                // инициализируем массив указателей-аргументов для функции execvp() для второй команды;
                char *args2[3] = {right_command[0], right_command[1], NULL};
                if (right_command[1][0] == '\0')
                    args2[1] = NULL;
                    
                if (execvp(right_command[0], args2) < 0) {
                    perror("EXECVP не удалось");
                    exit(EXIT_FAILURE);
                }
            } else {
                close(fd[0]);
                waitpid(pid2, &status2, 0); 
            }
        }
    }
}
