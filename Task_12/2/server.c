// сервер создаёт именованный канал и открывает его только на запись, записывает строку 'Hi!' и завершается
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int main(void) {
    char message[] = "Hi!";
    char *file_name = "msg_buf";
    int fd;
    
    if (mkfifo(file_name, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) < 0) {
        perror("Ошибка при создании канала!\n");
        exit(EXIT_FAILURE);
    }
    
    if ((fd = open(file_name, O_WRONLY)) < 0) {
        perror("Ошибка при открытии файла!");
        exit(EXIT_FAILURE);
    }
    
    
    write(fd, message, sizeof(message));
    close(fd);
    
    return 0;
}
