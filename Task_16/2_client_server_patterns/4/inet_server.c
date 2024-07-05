#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/epoll.h>

#define PORT 8888
#define MAX_EVENTS 10
// заметка (для себя):
// MAX_EVENTS - кол-во одновременно обрабатываемых epoll событий на сокетах.
// однако, если используем UDP сокет, то там происходит передача датаграмм => значение лучше увеличить,
// а то пакеты могут потеряться (вроде как чтение 1 пакета = 1 событие).
// если MAX_EVENTS < (полученные данные), то лишние данные потеряются и прочитаются в след раз.
// Слишком большое значение MAX_EVENTS кушает ресурс.
// (крч подстраиваться под кол-во фд, размер того, что читаем/записываем и как это происходит)

struct timeval get_current_time(); // нахождение текущего времени

int main(void) {
    // структуры сервера для адресов сетевого соединения
    struct sockaddr_in server_TCP, server_UDP;
    
    // создание TCP сокета
    int socket_TCP = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_TCP == -1) {
        perror("failed to create socket for TCP connection");
        exit(EXIT_FAILURE);
    }
    
    // настройка адреса TCP сокета сервера
    server_TCP.sin_family = AF_INET; // сетевой обмен
    server_TCP.sin_port = htons(PORT); // из литтл в биг
    server_TCP.sin_addr.s_addr = INADDR_ANY; // любой доступный адрес на хосте
    
    // привязка адреса сервера к TCP сокету
    if (bind(socket_TCP, (struct sockaddr *)&server_TCP, sizeof(server_TCP)) == -1) {
        perror("failed to binding address with TCP socket");
        close(socket_TCP);
        exit(EXIT_FAILURE);
    }
    
    // создание UDP сокета сервера
    int socket_UDP = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_UDP == -1) {
        perror("failed to create socket for UDP connection");
        close(socket_TCP);
        exit(EXIT_FAILURE);
    }
    
    // настройка адреса UDP сокета сервера
    server_UDP.sin_family = AF_INET; // сетевой обмен
    server_UDP.sin_port = htons(PORT); // из литтл в биг
    server_UDP.sin_addr.s_addr = INADDR_ANY; // любой доступный адрес на хосте
    
    // привязка адреса сервера к UDP сокету
    if (bind(socket_UDP, (struct sockaddr *)&server_UDP, sizeof(server_UDP)) == -1) {
        perror("failed to binding address with UDP socket");
        close(socket_TCP);
        close(socket_UDP);
        exit(EXIT_FAILURE);
    }
    
    // прослушивание TCP сокета
    if (listen(socket_TCP, 5) == -1) {
        perror("failed to listen TCP socket");
        close(socket_TCP);
        close(socket_UDP);
        exit(EXIT_FAILURE);
    }
    
    // структуры: ev - для конкретного события, events - для информации о группе событий
    struct epoll_event ev, events[MAX_EVENTS];
    
    // создание epoll-инстанции для мониторинга событий на файловых дескрипторах
    int epoll_fd = epoll_create1(0); // возвращает фд
    if (epoll_fd == -1) {
        perror("failed to create epoll");
        close(socket_TCP);
        close(socket_UDP);
        exit(EXIT_FAILURE);
    }
    
    // устанавливаем событие: чтение данных
    ev.events = EPOLLIN; // (если данные станут доступны для чтения, то epoll уведомит)
    
    // добавление TCP сокета сервера в epoll
    ev.data.fd = socket_TCP;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_TCP, &ev) == -1) {
        perror("failed to add socket to epoll");
        close(socket_TCP);
        close(socket_UDP);
        close(epoll_fd);
        exit(EXIT_FAILURE);
    }
    
    // добавление TCP сокета сервера в epoll
    ev.data.fd = socket_UDP;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_UDP, &ev) == -1) {
        perror("failed to add socket to epoll");
        close(socket_TCP);
        close(socket_UDP);
        close(epoll_fd);
        exit(EXIT_FAILURE);
    }
    
    // связь с клиентом
    while (1) {
        // ожидание события в любом из фд
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("failed to try wait");
            break;
        }
        
        // обработка TCP и UDP подключения
        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == socket_TCP) {
                // структура адреса клиента
                struct sockaddr_in client;
                
                // размер адреса клиента
                socklen_t client_len = sizeof(client);
                
                // установка соединения с клиентом
                int new_socket = accept(socket_TCP, (struct sockaddr *)&client,  &client_len);
                if (new_socket == -1) {
                    perror("TCP connection failed");
                    break;
                }
                
                // получение текущего времени
                struct timeval tv = get_current_time();
                if (tv.tv_sec == -1) {
                    perror("wrong time");
                    break;
                }
                
                // отправка текущего времени клиенту
                if(send(new_socket, (void *)&tv, sizeof(tv), 0) == -1) {
                    perror("failed to sending msg");
                    close(new_socket);
                    break;
                }
            } else if (events[i].data.fd == socket_UDP) {
                // структура адреса клиента
                struct sockaddr_in client;
                
                // размер адреса клиента
                socklen_t client_len = sizeof(client);
                
                // буффер - КПП (просто для приёма)
                char buffer[1] = { 0 };
                
                // чтение данных
                int bytes_read = recvfrom(socket_UDP, buffer, 1, 0, (struct sockaddr *)&client, &client_len);
                if (bytes_read == -1) {
                    perror("failed to receive");
                    break;
                } else if (bytes_read < 1) {
                    perror("wrong msg size");
                    break;
                }
                
                // получение текущего времени
                struct timeval tv = get_current_time();
                if (tv.tv_sec == -1) {
                    perror("wrong time");
                    break;
                }
                
                // отправка текущего времени клиенту
                if (sendto(socket_UDP, (void*)&tv, sizeof(tv), 0, (struct sockaddr *)&client, client_len) == -1) {
                    perror("failed to sending msg");
                    break;
                }
            }
        }
    }
    
    close(socket_TCP);
    close(socket_UDP);
    close(epoll_fd);
    
    return 0;
}

struct timeval get_current_time() {
    struct timeval tv;
    
    gettimeofday(&tv, NULL);
    if (tv.tv_sec == -1) {
        perror("failed getting time of day");
        tv.tv_sec = -1;
        tv.tv_usec = 0;
    }
    
    return tv;
}
