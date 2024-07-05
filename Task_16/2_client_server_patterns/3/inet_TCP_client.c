#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h> // для поиска адреса сервера
#include <sys/time.h>
#include <time.h>

#define PORT 8888
#define DOMEN "localhost"
#define SERVER_IP "127.8.8.8"

int main(void) {
    struct sockaddr_in server; // объявление структуры адреса сокета сервера
    socklen_t server_len = sizeof(server);
    
    struct timeval tv; // структура, в которую прочитаем данные из сокета
    
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0); // получение файлового дескриптора сокета
    if (sock_fd == -1) {
        perror("failed to get socket fd");
        exit(EXIT_FAILURE);
    } // если не удалось получить фд сокета, выводим сообщение об ошибке и завершаем программу
    
    // настройка адреса для сокета сервера
    server.sin_family = AF_INET; // указываем семейство
    server.sin_port = htons(PORT); // определяем порт
    
    // в server.sin_addr записываем IP-адрес в формате bigendian
    int s = inet_pton(AF_INET, SERVER_IP, &server.sin_addr);
    if (s <= 0) {
        if (s == 0)
            perror("not in presentation format");
        else
            perror("inet_pton");
        exit(EXIT_FAILURE);
    }
    
    if (connect(sock_fd, (struct sockaddr *)&server, server_len) == -1) {
        perror("connection failed");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }
    
    ssize_t bytes_recv = recv(sock_fd, (void *)&tv, sizeof(tv), 0);
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


