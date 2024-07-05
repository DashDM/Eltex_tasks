#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h> // для поиска свободного адреса
#include <pthread.h>
#include <sys/time.h>

#define PORT 8888
#define DOMEN "localhost"

void* client_service(void* arg); // обслуживание клиента

int main(void) {
    // структуры для адреса сервера и клиента
    struct sockaddr_in server, client;
    
    // размер адреса клиента
    socklen_t client_len = sizeof(client);
    
    // идентификатор потока 
    pthread_t thread_id = 0;
    
    // создание файлового дескриптора сервера
    int passive_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (passive_sock_fd == -1) {
        perror("failed to create socket");
        exit(EXIT_FAILURE);
    }
    
    // настройка адреса сервера
    server.sin_family = AF_INET; // определяем семейство для межсетевого взаимодействия
    server.sin_port = htons(PORT); // в момент соединения назначаем порт
    server.sin_addr.s_addr = INADDR_ANY; // в момент соединения назначаем любой свободный IP-адрес
    
     // привязка адреса сервера к TCP сокету
    if (bind(passive_sock_fd, (struct sockaddr *)&server, sizeof(server)) == -1) {
        perror("failed to binding server address");
        close(passive_sock_fd);
        exit(EXIT_FAILURE);
    }
    
    // прослушивание TCP сокета
    if (listen(passive_sock_fd, 5) == -1) {
        perror("failed to listening socket");
        close(passive_sock_fd);
        exit(EXIT_FAILURE);
    }
    
    // связь с клиентом
    while (1) {
        // установление соединения с пользователем
        int active_socket_fd = accept(passive_sock_fd, (struct sockaddr *)&client, &client_len);
        if (active_socket_fd == -1) {
            perror("connection failed");
            break;
        }
        
        // выделение памяти под фд нового сокета (чтобы избежать гонки между потоками)
        int *inbetween_fd = calloc(1, sizeof(int));
        if (inbetween_fd == NULL) {
            perror("failed to allocate memory");
            close(active_socket_fd);
            break;
        }
        
        // копируем активный фд сокета
        *inbetween_fd = active_socket_fd;
        
        // создание обслужающего сервера
        if (pthread_create(&thread_id, NULL, client_service, (void *)inbetween_fd) != 0) {
            perror("failed to create a pthread");
            close(active_socket_fd);
            free(inbetween_fd);
            break;
        }
        
        // для асинхронной работы делаем потоки самостоятельными (чтобы закрывались сами, а не ждали новых для этого)
        pthread_detach(thread_id);
    }
    
    close(passive_sock_fd);
    
    return 0;
}

void* client_service(void* arg) {
    int inbetween_fd = *(int *)arg;
    
    struct timeval tv;
    time_t time = 0;
    
    time = gettimeofday(&tv, NULL);
    if (time == -1) {
        perror("failed getting time of day");
        close(inbetween_fd);
        free(arg);
        exit(EXIT_FAILURE);
    }
    
    if(send(inbetween_fd, (void *)&tv, sizeof(tv), 0) == -1) {
        perror("failed to sending msg");
        close(inbetween_fd);
        free(arg);
        exit(EXIT_FAILURE);
    }
    
    close(inbetween_fd); // закрываем активный фд сокета
    free(arg); // освобождаем выделенную под фд память
    
    return NULL;
}


