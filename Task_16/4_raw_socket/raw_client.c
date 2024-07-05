#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 8888
#define CLIENT_IP "127.8.8.8"

int main(void) {
    // массив для данных
    char buffer[64] = { 0 };
    char str[] = "HELLO";
    
    // адрес raw сокета и мнимый адрес сервера (отправителя)
    struct sockaddr_in client, server;
    socklen_t server_len = sizeof(server);
    
    // создание raw-сокета
    int socket_fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (socket_fd < 0) {
        perror("failed to create raw socket");
        exit(EXIT_FAILURE);
    }
    
    // настройка адреса для сокета
    client.sin_family = AF_INET;
    client.sin_port = htons(PORT);
    
    int s = inet_pton(AF_INET, CLIENT_IP, &client.sin_addr);
    if (s <= 0) {
        if (s == 0)
            perror("not in presentation format");
        else
            perror("failed to pton IP-address");
        
        close(socket_fd);
        exit(EXIT_FAILURE);
    }
    
    // получение пакетов с сетевого уровня
    while (1) {
        // приём пакетов данных
        int burst_size = recvfrom(socket_fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&server, &server_len);
        if (burst_size == 0) {
            perror("failed to receive");
            break;
        } else if (burst_size > sizeof(buffer)) {
            printf("wrong buffer size: %d\n", burst_size);
        }
        
        printf("TAKE PACKEGE FROM: port -> %d | ip -> %s\n",  ntohs(server.sin_port), inet_ntoa(server.sin_addr));
    }
    
    close(socket_fd);
    
    return 0;
}
