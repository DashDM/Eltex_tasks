#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8888
#define MULTICAST_IP "228.8.8.8" // вроде от 224.0.0.1 до 239.255.255.255 - IP-адреса для мультикаста

int main(void) {
    // модифицированный широковещательный адрес
    struct sockaddr_in multicast;
    socklen_t multicast_len = sizeof(multicast);
    
    // создание сокета сервера
    int socket_server = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_server < 0) {
        perror("failed to create socket");
        exit(EXIT_FAILURE);
    }
    
    // настройка адреса
    multicast.sin_family = AF_INET;
    multicast.sin_port = htons(PORT);
    
    int s = inet_pton(AF_INET, MULTICAST_IP, &multicast.sin_addr);
    if (s <= 0) {
        if (s == 0)
            perror("not in presentation format");
        else
            perror("failed to pton IP-address");
        exit(EXIT_FAILURE);
    }
    
    char buff[32] = "Oh, Mamma mia, I did it!";
    
    // отправка сообщения клиенту
    if (sendto(socket_server, buff, sizeof(buff), 0, (struct sockaddr *)&multicast, multicast_len) == -1) {
        perror("failed to sending msg");
        close(socket_server);
        exit(EXIT_FAILURE);
    }
    
    close(socket_server);
    
    return 0;
}
