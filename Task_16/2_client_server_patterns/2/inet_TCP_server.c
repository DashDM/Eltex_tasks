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

#define BASE_PORT 8000
#define DOMEN "localhost"
#define THREAD_ARRAY_SIZE 5

struct thread_info {
    int port; // 8001 - порт первого потока
    int status; // (1 - cвободен, 0 - занят)
};

struct thread_info *shm_address = 0; // указатель на адрес сегмента разделяемой памяти

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
    
    // создание потоков и запись массива характеристик в сегмент разделяемой памяти
    for (int i = 0; i < THREAD_ARRAY_SIZE; i++) {
        shm_address[i].status = 1;
        shm_address[i].port = BASE_PORT + i + 1;
        
        int *arg = malloc(1);
        *arg = i; // id потока == ячейка массива в сегменте
        
        if (pthread_create(&thread_id[i], NULL, service, arg) != 0) {
            perror("failed to create pthread");
            free(thread_id);
            shm_unlink("/my_shm");
            exit(EXIT_FAILURE);
        }
        
        sleep(1);
        
        free(arg);
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
        shm_unlink("/my_shm");
        exit(EXIT_FAILURE);
    }
    
    // настройка адреса сервера
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(BASE_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    
    // привязка адреса сервера к сокету
    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) ==  -1) {
        perror("failed to bind server address");
        close(server_sock);
        free(thread_id);
        shm_unlink("/my_shm");
        exit(EXIT_FAILURE);
    }
    
    // прослушивание сокета сервера
    if (listen(server_sock, 5) == -1) {
        perror("failed to listen socket");
        close(server_sock);
        free(thread_id);
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
                shm_address[i].status = 0;
                if(send(new_sock, &shm_address[i].port, sizeof(int), 0) == -1) {
                    perror("failed to sending msg");
                    break;
                }
            }
        }
        
        close(new_sock);
    }
    
    close(server_sock);
    free(thread_id);
    
    // удаляем отображение сегмента разделяемой памяти из текущего процесса
    if (munmap(shm_address, THREAD_ARRAY_SIZE) < 0) {
        perror("munmap");
    }
    
    // полностью удаляем сегмент разделяемой памяти
    if (shm_unlink("/my_shm") < 0) {
        perror("shm_unlink");
    }
    
    return 0;
}

void *service(void *arg) {
    // получаем идентификатор потока в сегменте shm
    int thread_id = *(int*)arg;
    
    // адрес обслуживающего сервера и клента
    struct sockaddr_in server_addr, client_addr;
    
    // размер адреса клиента
    socklen_t client_addr_len = sizeof(client_addr);
    
    // создание сокета обслуживающего сервера
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        perror("failed to create socket");
        pthread_exit(NULL);
    }
    
    printf("%d\n", shm_address[thread_id].port);
    
    // настройка адреса сервера
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(shm_address[thread_id].port);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    
    // привязка адреса сервера к сокету
    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) ==  -1) {
        perror("failed to bind server address");
        close(server_sock);
        pthread_exit(NULL);
    }
    
    // прослушивание сокета сервера
    if (listen(server_sock, 5) == -1) {
        perror("failed to listen socket");
        close(server_sock);
        pthread_exit(NULL);
    }
    
    // связь с клиентом
    while (1) {
        // создание активного сокета сервера
        int new_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_len);
        if (new_sock == -1) {
            perror("connection failed");
            break;
        }
        
        // поиск текущего времени
        struct timeval tv;
        gettimeofday(&tv, NULL);
        
        if (tv.tv_sec == -1) {
            perror("failed getting time of day");
            pthread_exit(NULL);
        }
        
        // отправка текущего времени клиенту
        if(send(new_sock, (void *)&tv, sizeof(tv), 0) == -1) {
            perror("failed to sending msg");
            pthread_exit(NULL);
        } else {
            shm_address[thread_id].status = 1;
        }
        
        close(new_sock);
        sleep(2);
    }
    
    close(server_sock);
    
    return NULL;
}
