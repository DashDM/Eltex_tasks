#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8888
#define CLIENT_IP "228.8.8.8"

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
    client.sin_addr.s_addr = INADDR_ANY;
    
    // связка адреса клиента с сокетом
    if (bind(socket_client, (struct sockaddr *)&client, sizeof(client)) < 0) {
        perror("failed to bind addr to socket");
        close(socket_client);
        exit(EXIT_FAILURE);
    }
    
    // присоединение к мультикаст-группе
    struct ip_mreqn multigroup;
    
    // (использую старый вариант, т.е. интерфейс присваивается к IP-адресу, а не указывается отдельным полем)
    // настройка мультигруппы
    multigroup.imr_address.s_addr = INADDR_ANY;
    
    int s = inet_pton(AF_INET, CLIENT_IP, & multigroup.imr_multiaddr.s_addr);
    if (s <= 0) {
        if (s == 0)
            perror("not in presentation format");
        else
            perror("failed to pton IP-address");
        
        close(socket_client);
        exit(EXIT_FAILURE);
    }
    
    // установка разрешения на мультикаст вещание
    if (setsockopt(socket_client, IPPROTO_IP, IP_ADD_MEMBERSHIP, &multigroup, sizeof(multigroup)) < 0) {
        perror("permission failed... just the suffer");
        close(socket_client);
        exit(EXIT_FAILURE);
    }
    
    // буфер для чтения
    char buffer[32] = { 0 };
    
    // чтение данных
    int bytes_read = recvfrom(socket_client, buffer, sizeof(buffer), 0, (struct sockaddr *)&server, &server_len);
    if (bytes_read < 0) {
        perror("failed to receive");
        close(socket_client);
        
        // удаление клиента из группы
        if (setsockopt(socket_client, IPPROTO_IP, IP_DROP_MEMBERSHIP, &multigroup, sizeof(multigroup)) < 0) {
            perror("unpermission failed... just the suffer");
        }
        
        exit(EXIT_FAILURE);
    } else if (bytes_read < sizeof(buffer)) {
        perror("wrong msg size");
        close(socket_client);
        
        // удаление клиента из группы
        if (setsockopt(socket_client, IPPROTO_IP, IP_DROP_MEMBERSHIP, &multigroup, sizeof(multigroup)) < 0) {
            perror("unpermission failed... just the suffer");
        }
        
        exit(EXIT_FAILURE);
    }
    
    // вывод сообщения на экран:
    printf("broadcast_server: %s\n", buffer);
    
    // удаление клиента из группы
    if (setsockopt(socket_client, IPPROTO_IP, IP_DROP_MEMBERSHIP, &multigroup, sizeof(multigroup)) < 0) {
        perror("unpermission failed... just the suffer");
    }
    
    close(socket_client);
    
    return 0;
}
