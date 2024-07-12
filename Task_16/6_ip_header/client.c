#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> // struct iphdr
#include <netinet/udp.h> // struct udphdr
#include <arpa/inet.h>
#include <string.h>

#define BUFF_SIZE 512

#define SERVER_IP "127.8.8.8"
#define SERVER_PORT 8888

#define CLIENT_IP "127.7.7.7"
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
    
    // создание IP заголовка
    struct iphdr ip_header = {
        .ihl = 5, // размер IP заголовка (считается так: version * ihl = size)
        .version= 4, // версия протокола IP
        .tos = 0, // байт для приоритета пакетов (ставим 0, т.к. неприспособленный компутир не потянет)
        .tot_len = htons(sizeof(struct udphdr) + sizeof(msg) + sizeof(struct iphdr)), // общая длина UDP пакета
        .id = htons(8888), // идентификатор для сборки фрагментированных пакетов
        .frag_off = htons(0), // флаг для настройки фрагментации (0 - фрагмент всего 1)
        .ttl = 255, // макс. кол-во промежуточных узлов (каждый узел делает -1)
        .protocol = IPPROTO_UDP, // поле, которое кодирует протокол, инкапсулируемый на транспортном уровне
        .check = 0, // чек-сумма
        .saddr = inet_addr(CLIENT_IP), // IP-адрес клиента
        .daddr = inet_addr(SERVER_IP) // IP-адрес сервера
    };
    
    // создание UDP пакета
    memcpy(datagram_send, &ip_header, sizeof(ip_header)); // первые 20 байт под IP заголовок
    memcpy(datagram_send + sizeof(ip_header), &udp_header, sizeof(udp_header)); // следующие 8 байт под UDP заголовок
    memcpy(datagram_send + sizeof(ip_header) + sizeof(udp_header), msg, sizeof(msg)); // после заголовка идёт сообщение
    
    // создание raw-сокета для назначения своего UDP заголовка (транспортный уровень)
    int sock_fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (sock_fd < 0) {
        perror("failed to create raw socket");
        exit(EXIT_FAILURE);
    }
    
    // модификация raw-сокета для назначения своего IP заголовка (сетевой уровнь)
    int flag = 1;
    if (setsockopt(sock_fd, IPPROTO_IP, IP_HDRINCL, &flag, sizeof(flag)) < 0) {
        perror("failed to modify socket with unic IP-header");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }
    
    // отправка данных серверу
    if (sendto(sock_fd, datagram_send, sizeof(datagram_send), 0, (struct sockaddr *)&to_server, to_server_len) < 0) {
        perror("failed to send datagram");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }
    
    // данные клиента
    printf("Client data: port -> %d | IP -> %s | msg -> %s\n", CLIENT_PORT, CLIENT_IP, msg);
    printf("________________________________________________________________\n");
    
    // получение ответа
    while (1) {
        int bytes_recv = recvfrom(sock_fd, datagram_take, sizeof(datagram_take), 0, (struct sockaddr *)&from_server, &from_server_len);
        if (bytes_recv < 0) {
            perror("failed to receive data");
            close(sock_fd);
            exit(EXIT_FAILURE);
        }
        
        /* обработка ответного пакета: */
        // преобразуем в IP-header
        struct iphdr *ip_header_recv = (struct iphdr *)datagram_take;
        
        // IP отправителя (от кого)
        char s_ip_recv[20] = { 0 };
        if (inet_ntop(AF_INET, &ip_header_recv->saddr, s_ip_recv, sizeof(s_ip_recv)) == NULL) {
            perror("failed to ntop S_IP");
            continue;
        }
        
        // IP получателя (кому)
        char d_ip_recv[20] = { 0 };
        if (inet_ntop(AF_INET, &ip_header_recv->daddr, d_ip_recv, sizeof(d_ip_recv)) == NULL) {
            perror("failed to ntop D_IP");
            continue;
        }
        
        // преобразуем в UDP-header [+ (ip_header_recv->ihl * 4), чтобы перейти в начало UDP заголовка]
        struct udphdr *udp_header_recv = (struct udphdr *)(datagram_take + (ip_header_recv->ihl * 4));
        
        // вывод данных отправителя
        printf("Receive data from: S_IP -> %s | D_IP -> %s | S_PORT -> %d | D_PORT -> %d\n",
                s_ip_recv, d_ip_recv, ntohs(udp_header_recv->uh_sport), ntohs(udp_header_recv->uh_dport));
        
        // вывод PAYLOAD пакета, если содержание предназначено для данного клиента
        if ((ip_header_recv->daddr == ip_header.saddr) && (udp_header_recv->uh_dport == udp_header.uh_sport)) {
            printf("Received: %d, %s\n\n", bytes_recv, datagram_take + 28); // (порядок такой -> [IPv4: +20 байт | udp_header: +8 байт | msg: начинается от 28 байта])
        } else {
            printf("...\n");
        }
    }
    
    close(sock_fd);
    
    return 0;
}
