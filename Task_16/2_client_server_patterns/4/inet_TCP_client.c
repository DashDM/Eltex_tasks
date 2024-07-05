#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>
#include <time.h>

#define PORT 8888
#define DOMEN "localhost"

int main(void) {
    // структура для адреса сервера
    struct sockaddr_in server;
    
    // размер адреса сервера
    socklen_t server_len = sizeof(server);
    
    // структура для чтения текущего времени из сокета
    struct timeval tv;
    
    // создание файлового дескриптора сокета
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        perror("failed to get socket fd");
        exit(EXIT_FAILURE);
    }
    
    // получаем информацию о хосте
    struct hostent *host_info = gethostbyname(DOMEN);
    if (host_info == NULL) {
        perror("can't get host information");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }
    
    // настройка адреса для сокета сервера
    server.sin_family = AF_INET; // назначаем семейство для сетевого обмена
    server.sin_port = htons(PORT); // назначаем порт
    server.sin_addr.s_addr = *(in_addr_t *)host_info->h_addr; // назначаем IP-адрес сервера
    
    // подключение к серверу
    if (connect(sock_fd, (struct sockaddr *)&server, server_len) == -1) {
        perror("connection failed");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }
    
    // чтение данных из сокета
    ssize_t bytes_recv = recv(sock_fd, (void *)&tv, sizeof(tv), 0);
    if(bytes_recv == -1) {
        perror("receiving failed. code: fatal error");
        close(sock_fd);
        exit(EXIT_FAILURE);
    } else if (bytes_recv != sizeof(tv)) {
        perror("receiving failed. code: wrong size");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }
    
    // перевод секунд в календарное время
    struct tm *tm = localtime((time_t *)&tv.tv_sec);
    
    // массив для хранения времени в строковом формате
    char time_str[32] = { 0 };
    
    // перевод времени в строковый формат
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tm);
    
    // вывод на экран
    printf("Current time: %s.%06ld\n", time_str, tv.tv_usec);
    
    close(sock_fd);
    
    return 0;
}


