#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> // struct iphdr
#include <netinet/udp.h> // struct udphdr
#include <netinet/ether.h> // struct ethhdr
#include <net/if.h> // if_nametoindex
#include <linux/if_packet.h> // struct sockaddr_ll
#include <arpa/inet.h>
#include <string.h>

// P.S.:
// проверить работу клиента не удалось, т.к. у меня слабенький ноут и
// от двух запущенных виртуалок сносит крышу. Просто у меня основа - винда,
// все дз делаю в виртуальной убунту. А если запустить одновременно 2 виртуалки, то
// в какой-то момент абсолютно всё зависает.
// Например, браузер открывается ~10 минут и глохнет ;(
// В целом, думаю, код рабочий. По крайней мере, я на это надеюсь..
// и на последок: вероятно перенос строк получился не очень удачный, однако
// опирался исключительно на google style

#define BUFF_SIZE 512

#define SERVER_IP "127.8.8.8"
#define SERVER_PORT 8888

#define CLIENT_IP "127.7.7.7"
#define CLIENT_PORT 7777

int main(void) {
    /* инициализация массивов */
    char datagram_send[BUFF_SIZE] = { 0 };  // массив для отправки сообщений
    char datagram_take[BUFF_SIZE] = { 0 }; // массив для приёма сообщений
    char msg[] = "hochu pizzu";
    
    /* создание и настройка адреса сервера */
    struct sockaddr_ll to_server, from_server;
    socklen_t to_server_len = sizeof(to_server);
    socklen_t from_server_len = sizeof(from_server);
    
    memset(&to_server, 0, sizeof(to_server)); // зануление структуры 
    to_server.sll_family = AF_PACKET;
    to_server.sll_ifindex = if_nametoindex("enp0s3"); // назначение интерфейса
    to_server.sll_halen = ETH_ALEN; // длина мак адреса 
    
    // MAC-адрес получателя
    to_server.sll_addr[0] = 0x08;
    to_server.sll_addr[1] = 0x00;
    to_server.sll_addr[2] = 0x27;
    to_server.sll_addr[3] = 0x79;
    to_server.sll_addr[4] = 0x92;
    to_server.sll_addr[5] = 0x48;
    
    /* создание raw-сокета */
    int sock_fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sock_fd < 0) {
        perror("failed to create raw socket");
        exit(EXIT_FAILURE);
    }
    
    /* создание Ethernet заголовка */
    struct ethhdr eth_header = {
        .h_dest = { 0x08, 0x00, 0x27, 0x79, 0x92, 0x48 }, // MAC-адрес получателя
        .h_source = { 0x08, 0x00, 0x27, 0x3a, 0x40, 0xe0 }, // MAC-адрес отправителя 
        .h_proto = htons(ETH_P_IP) // тип инкапсулируемого протокола (IP)
    };
    
    /* создание UDP заголовка */
    struct udphdr udp_header = {
        .uh_sport = htons(CLIENT_PORT), // порт отправителя
        .uh_dport = htons(SERVER_PORT), // порт получателя
        .uh_ulen = htons(sizeof(struct udphdr) + sizeof(msg)), // размер заголовка + размер сообщения
        .uh_sum = 0 // контрольная сумма udp-заголовка
    };
    
    /* создание IP заголовка */
    struct iphdr ip_header = {
        .ihl = 5, // размер IP заголовка (version * ihl = size)
        .version= 4, // версия протокола IP
        .tos = 0, // байт для приоритета пакетов (ставим 0, т.к. неприспособленный компутир не потянет)
        .tot_len = htons(sizeof(struct udphdr) + sizeof(msg) + sizeof(struct iphdr)),
        .id = htons(8888), // идентификатор для сборки фрагментированных пакетов
        .frag_off = htons(0), // флаг для настройки фрагментации
        .ttl = 255, // макс. кол-во промежуточных узлов
        .protocol = IPPROTO_UDP, // поле, которое кодирует протокол, инкапсулируемый на транспортном уровне
        .check = 0, // чек-сумма
        .saddr = inet_addr(CLIENT_IP), // IP-адрес клиента
        .daddr = inet_addr(SERVER_IP) // IP-адрес сервера
    };
    
    /* опредление чек-суммы */
    char buf[128] = { 0 }; // массив для хранения IP заголовка
    int csum = 0; // контрольное значение чек-суммы
    short *ptr = (short *)buf; // двухбайтовый чтец заголовка
    
    memcpy(buf, &ip_header, sizeof(ip_header));
    
    // двухбайтное сложение IP заголовка
    for (int i = 0; i < 10; i++) {
        csum += *ptr++;
    }
    
    csum = (csum & 0xFFFF) + (csum >> 16); // учитываем переполнение и добавляем единицы обратно
    csum = ~csum; // инвертируем чек-сумму
    
    ip_header.check = htons(csum); // запись чек-суммы в IP заголовок
    
    /* создание UDP пакета */
    memcpy(datagram_send, &eth_header, sizeof(eth_header)); // первые 6 байт под Ethernet
    
    memcpy(datagram_send + sizeof(eth_header),
        &ip_header, sizeof(ip_header)); // след 20 байт под IP заголовок
           
    memcpy(datagram_send + sizeof(eth_header) + sizeof(ip_header), 
        &udp_header, sizeof(udp_header)); // 8 байт под заголовок udp 
           
    memcpy(datagram_send + sizeof(eth_header) + sizeof(ip_header) + sizeof(udp_header), 
        &msg, sizeof(msg)); // и payload
    
    /* отправка UDP пакета серверу */
    if (sendto(sock_fd, datagram_send, sizeof(datagram_send), 0, 
        (struct sockaddr *)&to_server, to_server_len) < 0) {
        perror("failed to send datagram");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }
    
    /* данные клиента */
    printf("Client data: MAC -> %s | IP -> %s | port -> %d | msg -> %s\n",
           "08:00:27:3a:40:e0", CLIENT_IP, CLIENT_PORT, msg);
    printf("________________________________________________________________\n");
    
    /* получение ответа */
    while (1) {
        int bytes_recv = recvfrom(sock_fd, datagram_take, sizeof(datagram_take), 
            0, (struct sockaddr *)&from_server, &from_server_len);
            
        if (bytes_recv < 0) {
            perror("failed to receive data");
            close(sock_fd);
            exit(EXIT_FAILURE);
        }
        
        /* обработка ответного пакета */
        // сравнение MAC-адресов
        struct ethhdr *eth_header_recv = (struct ethhdr *)datagram_take;
        
        if (memcmp(eth_header_recv->h_dest, eth_header.h_source, ETH_ALEN) != 0) {
            continue;
        }
        
        // сравнение IP-адресов
        struct iphdr *ip_header_recv = 
            (struct iphdr *)(datagram_take + sizeof(struct ethhdr));
        
        if (ip_header_recv->daddr != ip_header.saddr) {
            continue;
        }
        
        // сраввнение UDP-портов
        struct udphdr *udp_header_recv = 
            (struct udphdr *)(datagram_take + sizeof(struct ethhdr) + sizeof(struct iphdr));
        
        if (udp_header_recv->uh_dport != udp_header.uh_sport ||
            udp_header_recv->uh_sport != udp_header.uh_dport) {
            continue;
        }
        
        // вывод содержимого
        printf("Received: %d, %s\n\n", bytes_recv, datagram_take 
            + sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr));
    }
    
    close(sock_fd);
    
    return 0;
}

