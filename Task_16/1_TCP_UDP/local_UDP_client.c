#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>

#define SERVER_SOCKET_PATH "/tmp/my_server_socket_UDP"
#define CLIENT_SOCKET_PATH "/tmp/my_client_socket_UDP"
#define BUFF_SIZE 32

int main(void) {
    char buffer_send[] = "Wassup, Server!";
    char buffer_recv[BUFF_SIZE] ={0};
    
    struct sockaddr_un server, client;
    
    socklen_t server_len = sizeof(server);
    
    // создание файлового дескриптора сокета
    int socket_fd = socket(AF_LOCAL, SOCK_DGRAM, 0);
    if (socket_fd < 0) {
        perror("socket: ошибка при создании сокета");
        exit(EXIT_FAILURE);
    }
    
    // настройка адреса сокета сервера
    server.sun_family = AF_LOCAL;
    strcpy(server.sun_path, SERVER_SOCKET_PATH);
    
    // настройка адреса сокета клиента
    client.sun_family = AF_LOCAL;
    strcpy(client.sun_path, CLIENT_SOCKET_PATH);
    
    // назначение адреса сокету клиента
    if (bind(socket_fd, (struct sockaddr *)&client, sizeof(client)) < 0) {
        perror("bind: не удалось назначить адрес сокету клиента");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }
    
    // отправка данных серверу
    if (sendto(socket_fd, buffer_send, sizeof(buffer_send), 0, (struct sockaddr *)&server, server_len) < 0) {
        perror("sendto: ошибка при отправки данных на сервер");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }
    
    // чтение ответа от сервера
    if (recvfrom(socket_fd, buffer_recv, sizeof(buffer_recv), 0, (struct sockaddr *)&server, &server_len) < 0) {
        perror("recvfrom: ошибка чтения данных из сокета");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }
    
    printf("Received: %s\n", buffer_recv);
    
    sleep(2);
    
    close(socket_fd);
    
    unlink(CLIENT_SOCKET_PATH);
    
    return 0;
}
