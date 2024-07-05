#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h> // для работы с сетевыми адресами
#include <arpa/inet.h> // для работы с IP - адресами

#define BUFF_SIZE 32

#define PORT 7777 // сервер слушает на данном порту
#define SERVER_IP "127.0.0.1" // и на данном адресе

int main(void) {
    char buffer_send[] = "Hi, Client!";
    char buffer_recv[BUFF_SIZE] = { 0 };
    
    struct sockaddr_in server_addr;
    socklen_t server_addr_len = sizeof(server_addr);
    
    // создание файлового дескрпитора сокета
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        perror("socket: creation error");
        exit(EXIT_FAILURE);
    }
    
    // настройка адреса сокета сервера
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) < 0) {
        perror("inet_pton: invalid address");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }
    
    // назначение сокету адреса 
    if (bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind: failed");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }
    
    // прослушивание входящего соединения
    if (listen(socket_fd, 1) < 0) {
        perror("listen: failed");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }
    
    // установка соединения с клиентом
    int new_socket_fd = accept(socket_fd, (struct sockaddr *)&server_addr, &server_addr_len);
    if (new_socket_fd < 0) {
        perror("accept: connection failed");
        close(new_socket_fd);
        close(socket_fd);
        exit(EXIT_FAILURE);
    }
    
    // чтение данных
    if (recv(new_socket_fd, buffer_recv, sizeof(buffer_recv), 0) < 0) {
        perror("recv: failed");
        close(new_socket_fd);
        close(socket_fd);
        exit(EXIT_FAILURE);
    }
    
    printf("Received: %s\n", buffer_recv);
    
    // отправка ответа
    if (send(new_socket_fd, buffer_send, sizeof(buffer_send), 0) < 0) {
        perror("send: failed");
        close(new_socket_fd);
        close(socket_fd);
        exit(EXIT_FAILURE);
    }
    
    sleep(5);
    
    close(new_socket_fd);
    close(socket_fd);
    
    return 0;
}
