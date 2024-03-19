#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>

#include "operations_third.h"

int main(void) {
    // инициализируем массив символов (содержит путь к программе)
    char path[MAX_PATH_LENGTH];
    
    printf("Принцип работы программы:\n1.Укажите путь к файлу (например, /home/dmitry/Desktop/eltex_homework\n2.Введите имя исполняемой программы и соответствующие атрибуты.\n3.Для выхода введите 'exit'\n\n");
    
    while (1) {
        // считываем строку из стандартного ввода stdin
        printf("Укажите путь к файлу (или введите 'exit' для выхода): ");
        fgets(path, MAX_PATH_LENGTH, stdin);
        // удаляем символ новой строки '\n', чтобы путь не содержал лишний символ
        del_newline_character(path);
        // проверка желает ли пользователь выйти (path = 'exit')
        if (compare_string(path) == 1) {
            break;
        }
        // инициализируем статус и порождаем новый процесс
        int status;
        pid_t pid = fork();
        // создание дочернего процесса с соответствующими проверками и ожиданием завершения
        if (pid == -1) {
            perror("fork");
            exit(0);
        } else if (pid == 0) {
            execl(path, path, NULL);
            perror("execl");
            exit(1);
        } else {
            wait(&status);
            printf("!status = %d\n\n", WEXITSTATUS(status));
        }
    }
    return 0;
}

