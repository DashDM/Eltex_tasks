#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h> // содержит объявление struct sockaddr_un
#include <string.h>

#define SERVER_SOCKET_PATH "/tmp/my_server_socket_TCP"
#define BUFF_SIZE 32

int main(void) {
    char buffer_recv[BUFF_SIZE] = {0};
    char buffer_send[] = "Hello, Client!";
    
    struct sockaddr_un server, client; // структуры, определяющие адрес сокетов
    
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
    
    // назначение адреса сокету сервера
    if (bind(socket_fd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("bind: не удалось назначить адрес сокету сервера");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }
    
    // прослушивание соединений (назначение socket_fd в качестве пассивного и организация очереди из 1 клиента)
    if (listen(socket_fd, 1) < 0) {
        perror("listen: не удалось определить соединение");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }
    
    // установление соединения
    int new_socket_fd = accept(socket_fd, (struct sockaddr *)&client, &client_len); // получаем активный дескриптор и становимся в ожидание клиента
    if (new_socket_fd < 0) {
        perror("accept: не удалось установить соединение с клиентом");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }
    
    // чтение данных
    if (recv(new_socket_fd, buffer_recv, sizeof(buffer_recv), 0) < 0) {
        perror("recv: ошибка чтения");
        close(new_socket_fd);
        close(socket_fd);
        exit(EXIT_FAILURE);
    }
    
    printf("Received: %s\n", buffer_recv);
    
    // отправка ответа
    if (send(new_socket_fd, buffer_send, sizeof(buffer_send), 0) < 0) {
        perror("send: ошибка отправки данных клиенту");
        close(new_socket_fd);
        close(socket_fd);
        exit(EXIT_FAILURE);
    }
    
    sleep(5);
    
    close(new_socket_fd);
    close(socket_fd);
    
    unlink(SERVER_SOCKET_PATH);
    
    return 0;
}
