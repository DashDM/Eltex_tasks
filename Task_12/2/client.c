// клиент открывает созданный канал сервером только на чтение, считывает строку из канала и выводит на экран. Удаляет канал.

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int main(void) {
    char message[3];
    char *file_name = "msg_buf";
    int fd;
    
    if ((fd = open(file_name, O_RDONLY)) < 0) {
        perror("Ошибка при открытии файла!");
        exit(EXIT_FAILURE);
    }
    
    
    read(fd, message, sizeof(message));
    printf("%s\n", message);
    close(fd);
    unlink(file_name);
    
    return 0;
}
