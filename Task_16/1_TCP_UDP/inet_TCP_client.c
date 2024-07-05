#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFF_SIZE 32

#define PORT 7777 // клиент пытается подключиться к данному порту
#define SERVER_IP "127.0.0.1" // и к данному адресу

int main(void) {
    char buffer_send[] = "Hi, Server!";
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
    
    // подключение к серверу
    if(connect(socket_fd, (struct sockaddr *)&server_addr, server_addr_len) < 0) {
        perror("connect: connection failed");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }
    
    // отправка данных на сервер
    if (send(socket_fd, buffer_send, sizeof(buffer_send), 0) < 0) {
        perror("send: failed");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }
    
    // чтение ответа от сервера
    if (recv(socket_fd, buffer_recv, sizeof(buffer_recv), 0) < 0) {
        perror("recv: failed");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }
    
    printf("Received: %s\n", buffer_recv);
    
    sleep(2);
    
    close(socket_fd);
    
    return 0;
}
