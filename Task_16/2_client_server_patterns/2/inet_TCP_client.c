#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>
#include <time.h>

#define PORT 8000
#define DOMEN "localhost"

int main(void) {
    struct sockaddr_in server; // объявление структуры адреса сокета сервера
    socklen_t server_len = sizeof(server);
    
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        perror("failed to get socket fd");
        exit(EXIT_FAILURE);
    }

    struct hostent *host_info = gethostbyname(DOMEN);
    if (host_info == NULL) {
        perror("can't get host-server information");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }
    
    // настройка адреса для сокета сервера
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = *(in_addr_t *)host_info->h_addr;

    // подключаемся к порту 8000 изначально
    if (connect(sock_fd, (struct sockaddr *)&server, server_len) == -1) {
        perror("connection failed - 1");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    // получаем порт потока от сервера
    int thread_port = 0;
    ssize_t bytes_recv = recv(sock_fd, (void *)&thread_port, sizeof(int), 0);
    if (bytes_recv == -1) {
        perror("receiving failed. code: -1");
        close(sock_fd);
        exit(EXIT_FAILURE);
    } else if (bytes_recv != sizeof(int)) {
        perror("receiving failed. code: wrong size");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    // закрываем предыдущий сокет
    close(sock_fd);

    // создаем новый сокет и подключаемся к порту потока
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        perror("failed to get socket fd");
        exit(EXIT_FAILURE);
    }

    server.sin_port = htons(thread_port);
    if (connect(sock_fd, (struct sockaddr *)&server, server_len) == -1) {
        perror("connection failed - 2");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    
    struct timeval tv; // время в секундах и микросекундах
    
    bytes_recv = recv(sock_fd, (void *)&tv, sizeof(tv), 0);
    if(bytes_recv == -1) {
        perror("receiving failed. code: -1");
        close(sock_fd);
        exit(EXIT_FAILURE);
    } else if (bytes_recv != sizeof(tv)) {
        perror("receiving failed. code: wrong size");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }
    
    struct tm *tm = localtime((time_t *)&tv.tv_sec); // переводим секунды в календарное время
    
    char time_str[32] = { 0 };
    
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tm); // переводим время в строковый формат
    
    printf("Current time: %s.%06ld\n", time_str, tv.tv_usec);
    
    close(sock_fd);
    
    return 0;
}


