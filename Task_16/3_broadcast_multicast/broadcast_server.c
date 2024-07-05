#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8888
#define BROADCAST_IP "255.255.255.255"

int main(void) {
    // широковещательный адрес
    struct sockaddr_in broadcast;
    socklen_t broadcast_len = sizeof(broadcast);
    
    // создание сокета сервера
    int socket_server = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_server < 0) {
        perror("failed to create socket");
        exit(EXIT_FAILURE);
    }
    
    // настройка адреса
    broadcast.sin_family = AF_INET;
    broadcast.sin_port = htons(PORT);
    
    int s = inet_pton(AF_INET, BROADCAST_IP, &broadcast.sin_addr);
    if (s <= 0) {
        if (s == 0)
            perror("not in presentation format");
        else
            perror("failed to pton IP-address");
        exit(EXIT_FAILURE);
    }
    
    // разрешение на использование широкого вещания
    int flag = 1;
    if (setsockopt(socket_server, SOL_SOCKET, SO_BROADCAST | SO_REUSEADDR, &flag, sizeof(flag)) < 0) {
        perror("permission failed... just the suffer");
        close(socket_server);
        exit(EXIT_FAILURE);
    }
    
    char buff[32] = "Oh, Mamma mia, I did it!";
    
    // отправка сообщения клиенту
    if (sendto(socket_server, buff, sizeof(buff), 0, (struct sockaddr *)&broadcast, broadcast_len) == -1) {
        perror("failed to sending msg");
        close(socket_server);
        exit(EXIT_FAILURE);
    }
    
    close(socket_server);
    
    return 0;
}
