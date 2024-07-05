#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8888
#define CLIENT_IP "255.255.255.255"

int main(void) {
    // адрес клиента и мнимый адрес сервера
    struct sockaddr_in client, server;
    socklen_t server_len = sizeof(server);
    
    // создание сокета
    int socket_client = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_client < 0) {
        perror("failed to create socket");
        exit(EXIT_FAILURE);
    }
    
    // настройка адреса клиента
    client.sin_family = AF_INET;
    client.sin_port = htons(PORT);
    
    int s = inet_pton(AF_INET, CLIENT_IP, &client.sin_addr);
    if (s <= 0) {
        if (s == 0)
            perror("not in presentation format");
        else
            perror("failed to pton IP-address");
        
        close(socket_client);
        exit(EXIT_FAILURE);
    }
    
    // связка адреса клиента с сокетом
    if (bind(socket_client, (struct sockaddr *)&client, sizeof(client)) < 0) {
        perror("failed to bind addr to socket");
        close(socket_client);
        exit(EXIT_FAILURE);
    }
    
    char buffer[32] = { 0 };
    
    // чтение данных
    int bytes_read = recvfrom(socket_client, buffer, sizeof(buffer), 0, (struct sockaddr *)&server, &server_len);
    if (bytes_read < 0) {
        perror("failed to receive");
        close(socket_client);
        exit(EXIT_FAILURE);
    } else if (bytes_read < sizeof(buffer)) {
        perror("wrong msg size");
        close(socket_client);
        exit(EXIT_FAILURE);
    }
    
    // вывод сообщения на экран:
    printf("broadcast_server: %s\n", buffer);
    
    close(socket_client);
    
    return 0;
}
