#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h> // содержит объявление struct sockaddr_un
#include <string.h> // Валера, настало твоё небезопасное время

#define SERVER_SOCKET_PATH "/tmp/my_server_socket_TCP"
#define BUFF_SIZE 32

int main(void) {
    char buffer_recv[BUFF_SIZE] = {0};
    char buffer_send[] = "Hello, Server!";
    
    struct sockaddr_un server, client; // структуры, определяющие адрес сокетов
    
    socklen_t server_len = sizeof(struct sockaddr_un); // длина структуры сокета сервера
    
    // создание файлового дескриптора сокета
    int socket_fd = socket(AF_LOCAL, SOCK_STREAM, 0); // согласно man: создаём endpoint, хотя он назначится попозже
    if (socket_fd < 0) {
        perror("socket: ошибка при создании сокета");
        exit(EXIT_FAILURE);
    }
    
    // настройка адреса сокета сервера
    server.sun_family = AF_LOCAL; // указываем семейство
    strcpy(server.sun_path, SERVER_SOCKET_PATH); // указываем путь
    
    socklen_t client_len = sizeof(client); // необходимо для грамотной передачи размера структуры сокета клиента в фу-ию accept
    
    // подключение к серверу
    if (connect(socket_fd, (struct sockaddr *)&server, server_len) < 0) {
        perror("connect: не удалось подключиться к серверу");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }
    
    // отправка данных на сервер
    if (send(socket_fd, buffer_send, sizeof(buffer_send), 0) < 0) {
        perror("send: ошибка отправки данных на сервер");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }
    
    // чтение ответа от сервера
    if (recv(socket_fd, buffer_recv, sizeof(buffer_recv), 0) < 0) {
        perror("recv: ошибка чтения данных");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }
    
    printf("Received: %s\n", buffer_recv);
    
    sleep(2);
    
    close(socket_fd);
    
    return 0;
}
