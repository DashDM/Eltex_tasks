#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/udp.h> // struct udphdr
#include <arpa/inet.h>
#include <string.h>

#define BUFF_SIZE 512

#define SERVER_IP "127.8.8.8"
#define SERVER_PORT 8888

#define CLIENT_PORT 7777

int main(void) {
    // массив для отправки сообщений
    char datagram_send[BUFF_SIZE] = { 0 };
    
    // массив для приёма сообщений
    char datagram_take[BUFF_SIZE] = { 0 };
    
    // сообщение
    char msg[] = "hochu pizzu";
    
    // структура для адреса сервера
    struct sockaddr_in to_server, from_server;
    
    // размер адреса сервера
    socklen_t to_server_len = sizeof(to_server), from_server_len = sizeof(from_server);
    
    // настройка адреса сервера
    to_server.sin_family = AF_INET;
    to_server.sin_port = htons(SERVER_PORT);
    
    int s1 = inet_pton(AF_INET, SERVER_IP, &to_server.sin_addr);
    if (s1 <= 0) {
        if (s1 == 0)
            perror("not in presentation format");
        else
            perror("failed to pton IP-address");
        
        exit(EXIT_FAILURE);
    }
    
    // создание UDP заголовка
    struct udphdr udp_header = {
        .uh_sport = htons(CLIENT_PORT), // кто отправляет
        .uh_dport = htons(SERVER_PORT), // кому отправляет
        .uh_ulen = htons(sizeof(struct udphdr) + sizeof(msg)), // размер заголовка + размер сообщения
        .uh_sum = 0 // контрольная сумма
    };
    
    // создание UDP пакета
    memcpy(datagram_send, &udp_header, sizeof(udp_header)); // копируем первые 8 байт под UDP заголовок
    memcpy(datagram_send + sizeof(udp_header), msg, sizeof(msg)); // после заголовка идёт сообщение
    
    // создание raw сокета
    int sock_fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (sock_fd < 0) {
        perror("failed to create raw socket");
        exit(EXIT_FAILURE);
    }
    
    // отправка данных серверу
    if (sendto(sock_fd, datagram_send, sizeof(datagram_send), 0, (struct sockaddr *)&to_server, to_server_len) < 0) {
        perror("failed to send datagram");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }
    
    // данные клиента
    printf("Client data: port -> %d | IP -> %s | msg -> %s\n", CLIENT_PORT, "[см. в первый IP-адрес ниже]", msg);
    printf("________________________________________________________________\n");
    
    // получение ответа
    while (1) {
        int bytes_recv = recvfrom(sock_fd, datagram_take, sizeof(datagram_take), 0, (struct sockaddr *)&from_server, &from_server_len);
        if (bytes_recv < 0) {
            perror("failed to receive data");
            close(sock_fd);
            exit(EXIT_FAILURE);
        }
        
        // преобразование IP - адреса отправителя
        char ip_recv[20] = { 0 };
        if (inet_ntop(AF_INET, &from_server.sin_addr, ip_recv, sizeof(ip_recv)) == NULL) {
            perror("failed to ntop IP");
            continue;
        }
        
        // массив для проверки совпадения Source port - порта отправителя (из datagram_send) с Destination port - портом получателя (из datagram_take)
        char datagram_buffer[BUFF_SIZE] = { 0 };
        
        // отбрасываем первые 20 байт, чтобы получить udp заголовок
        for (int i = 0; i < sizeof(datagram_take) - 20; i++) {
            datagram_buffer[i] = datagram_take[i + 20];
        }
        
        // преобразуем промежуточный массив в структуру под udp заголовок
        struct udphdr *udp_header_serv = (struct udphdr *)datagram_buffer;
        
        // данные отправителя (сервера)
        printf("Sender data: source port -> %d | destination port -> %d | IP -> %s\n", ntohs(udp_header_serv->uh_sport), ntohs(udp_header_serv->uh_dport), ip_recv);
        
        // вывод последней части пакета, если содержание предназначено для данного клиента
        if (ntohs(udp_header_serv->uh_dport) == CLIENT_PORT) {
            printf("Received: %d, %s\n\n", bytes_recv, datagram_take + 28); // (порядок такой -> [IPv4: +20 байт | udp_header: +8 байт | msg: начинается от 28 байта])
        } else {
            printf("...\n");
        }
    }
    
    close(sock_fd);
    
    return 0;
}
