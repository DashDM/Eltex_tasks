#ifndef __OPERATIONS_THIRD_H__
#define __OPERATIONS__THIRD_H__
// на будущее, чтобы не забыть:
// MAX_ARGS можно попробовать объяввить глобально,
// и далее, читая посимвольнно введённую строку,
// присваивать MAX_ARGS соотв. значение [но потом]
#define MAX_ARGS 2 // сколько всего аргументов в строке (считать так: команда + атрибут1 + атрибут2 + ... + атрибутN)
#define MAX_INPUT_LENGTH 1024
#define MAX_CMD_SIZE 128
#define OPERATIONS_NUMS 5
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>

void my_bash(void);
void my_bash_input(char *string);
void del_newline_character(char *string);
void execute_command(char *string);
void partition_command(char *string, int separator, char *left_command[], char *right_command[]);
void start_command(int separator, char *left_command[], char *right_command[]);

#endif
