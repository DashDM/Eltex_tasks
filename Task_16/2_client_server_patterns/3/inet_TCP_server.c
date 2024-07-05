#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PORT 8888
#define DOMEN "localhost"
#define SERVER_IP "127.8.8.8"
#define THREAD_ARRAY_SIZE 5

struct thread_arg {
    int thread_id; // идентификатор потока
    int socket; // идентификатор активного сокета сервера
    int status; // состояние потока (1 - свободен, 0 - используется)
};

struct thread_arg *shm_address = 0; // указатель на адрес сегмента разделяемой памяти

void *service(void *arg);

int main(void) {
    // создание сегмента разделяемой памяти
    int shm_fd = shm_open("/my_shm", O_CREAT | O_RDWR, 0666);
    if (shm_fd < 0) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
    
    // усечение размера сегмента (файла)
    if (ftruncate(shm_fd, THREAD_ARRAY_SIZE) < 0) {
        perror("ftruncate");
        shm_unlink("/my_shm");
        exit(EXIT_FAILURE);
    }
    
    // отображение страниц в виртуальное адресное пространство программы
    shm_address = mmap(NULL, THREAD_ARRAY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_address == MAP_FAILED) {
        perror("mmap");
        shm_unlink("/my_shm");
        exit(EXIT_FAILURE);
    }
    
    // инициализация массива потоков
    pthread_t *thread_id = calloc(THREAD_ARRAY_SIZE, sizeof(pthread_t));
    if (thread_id == NULL) {
        perror("failed to allocate memory");
        shm_unlink("/my_shm");
        exit(EXIT_FAILURE);
    }
    
    // инициализация массива характеристик потоков
    struct thread_arg *thread_info = calloc(THREAD_ARRAY_SIZE, sizeof(struct thread_arg));
    if (thread_info == NULL) {
        perror("failed to allocate memory");
        free(thread_id);
        shm_unlink("/my_shm");
        exit(EXIT_FAILURE);
    }
    
    // создание потоков и запись массива характеристик в сегмент разделяемой памяти
    for (int i = 0; i < THREAD_ARRAY_SIZE; i++) {
        thread_info[i].thread_id = i;
        thread_info[i].socket = 0;
        thread_info[i].status = 1;
        
        shm_address[i] = thread_info[i]; // копируем элемент массива в сегмент shm
        
        if (pthread_create(&thread_id[i], NULL, service, &thread_info[i]) != 0) {
            perror("failed to create pthread");
            free(thread_id);
            free(thread_info);
            shm_unlink("/my_shm");
            exit(EXIT_FAILURE);
        }
    }
    
    // адрес сервера и клента
    struct sockaddr_in server_addr, client_addr;
    
    // размер адреса клиента
    socklen_t client_addr_len = sizeof(client_addr);
    
    // создание сокета сервера
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        perror("failed to create socket");
        free(thread_id);
        free(thread_info);
        shm_unlink("/my_shm");
        exit(EXIT_FAILURE);
    }
    
    // поиск свободного IP-адреса
    struct hostent *host_info = gethostbyname(DOMEN);
    if (host_info == NULL) {
        perror("can't get host-server information");
        close(server_sock);
        free(thread_id);
        free(thread_info);
        shm_unlink("/my_shm");
        exit(EXIT_FAILURE);
    }
    
    // настройка адреса сервера
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    
    // в server.sin_addr записываем IP-адрес в формате bigendian
    int s = inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);
    if (s <= 0) {
        if (s == 0)
            perror("not in presentation format");
        else
            perror("inet_pton");
        exit(EXIT_FAILURE);
    }
    
    
    // привязка адреса сервера к сокету
    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) ==  -1) {
        perror("failed to bind server address");
        close(server_sock);
        free(thread_id);
        free(thread_info);
        shm_unlink("/my_shm");
        exit(EXIT_FAILURE);
    }
    
    // прослушивание сокета сервера
    if (listen(server_sock, 5) == -1) {
        perror("failed to listen socket");
        close(server_sock);
        free(thread_id);
        free(thread_info);
        shm_unlink("/my_shm");
        exit(EXIT_FAILURE);
    }
    
    // связь с клиентом
    while (1) {
        // создание активного сокета сервера
        int new_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_len);
        if (new_sock == -1) {
            perror("connection failed");
            break;
        }
        
        // поиск свободного потока
        for (int i = 0; i < THREAD_ARRAY_SIZE; i++) {
            if (shm_address[i].status) {
                // в сегменте разделяемой памяти
                shm_address[i].status = 0; // перезаписываем статус
                shm_address[i].socket = new_sock; // перезаписываем сокет 
                
                // подаём сигнал в соответствующий поток (наткунлся в мануале и очень хочу попробовать)
                if (pthread_kill(thread_id[i], SIGUSR1) != 0) {
                    perror("failed to transfer signal to pthread");
                    break;
                }
            }
        }
    }
    
    close(server_sock);
    free(thread_id);
    free(thread_info);
    
    // удаляем отображение сегмента разделяемой памяти из текущего процесса
    if ( munmap(shm_address, THREAD_ARRAY_SIZE) < 0) {
        perror("munmap");
    }
    
    // полностью удаляем сегмент разделяемой памяти
    if (shm_unlink("/my_shm") < 0) {
        perror("shm_unlink");
    }
    
    return 0;
}

void *service(void *arg) {
    // объект с характеристиками потока (.thread_id, .socket, .status)
    struct thread_arg thread_info = *(struct thread_arg *)arg;
    
    // создаём маску сигналов
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    
    // устанавливаем обработчик сигнала (аналог sigprocmask)
    if(pthread_sigmask(SIG_BLOCK, &mask, NULL) != 0) {
        perror("failed to create signal handler");
        pthread_exit(NULL);
    }
    
    // обслуживание клиента
    while (1) {
        // ожидание сигнала от main-сервера (запомнить: в случае ошибки вернёт значение > 0)
        int sig;
        if (sigwait(&mask, &sig) != 0) {
            perror("failed to waiting signal");
            pthread_exit(NULL);
        }
        
        // поиск текущего времени
        struct timeval tv;
        gettimeofday(&tv, NULL);
        
        if (tv.tv_sec == -1) {
            perror("failed getting time of day");
            pthread_exit(NULL);
        }
        
        // отправка текущего времени клиенту
        if(send(shm_address[thread_info.thread_id].socket, (void *)&tv, sizeof(tv), 0) == -1) {
            perror("failed to sending msg");
            exit(EXIT_FAILURE);
        } else {
            shm_address[thread_info.thread_id].status = 1;
        }
    }
    
    return NULL;
}
