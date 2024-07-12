#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFF_SIZE 512

#define SERVER_IP "127.8.8.8"
#define SERVER_PORT 8888

int main(void) {
    // буфер для чтения данных
    char datagram[BUFF_SIZE] = { 0 };
    
    // добавочная строка
    char add[] = " - confirmed by the server";
    
    // структуры для адреса сервера и клиента
    struct sockaddr_in server, client;
    
    // размер адреса клиента
    socklen_t client_len = sizeof(client);
    
    // настройка адреса сервера
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    
    int s = inet_pton(AF_INET, SERVER_IP, &server.sin_addr);
    if (s <= 0) {
        if (s == 0)
            perror("not in presentation format");
        else
            perror("failed to pton IP-address");
        
        exit(EXIT_FAILURE);
    }
    
    // создание сокета сервера
    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd < 0) {
        perror("failed to create socket");
        exit(EXIT_FAILURE);
    }
    
    // назначение сокету адреса сервера
    if (bind(sock_fd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("failed to bind address");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }
    
    // связь с клиентом
    while (1) {
        // ожидание сообщения от клиента
        int bytes_recv = recvfrom(sock_fd, datagram, sizeof(datagram), 0,
            (struct sockaddr *)&client, &client_len);
            
        if (bytes_recv == 0) {
            perror("failed to receive data");
            break;
        }
        
        // модификация сообщения
        int msg_count = 0;
        
        // находим конец сообщения
        while (datagram[msg_count] != '\0')
            msg_count++;
            
        for (int i = 0; add[i] != '\0'; i++) {
            datagram[msg_count] = add[i];
            msg_count++;
        }
                
        datagram[msg_count] = '\0';
        
        // отправка данных клиенту
        int bytes_send = sendto(sock_fd, datagram, sizeof(datagram), 0, 
            (struct sockaddr *)&client, client_len);
            
        if (bytes_send < 0) {
            perror("failed to send datagram");
            close(sock_fd);
            continue;
        }
    }
    
    close(sock_fd);
    
    return 0;
}
