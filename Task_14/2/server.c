#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_NICKNAME 10
#define MAX_MSG 256

struct list {
    int pid;
    char nickname[MAX_NICKNAME];
};

struct history {
    char nickname[MAX_NICKNAME];
    char msg[MAX_MSG];
};

void *shm_list();
void *shm_history();

int main(void) {
    pthread_t threads[2]; // объявление массива потоков
    
    pthread_create(&threads[0], NULL, shm_list, NULL); // создание потока для работы со списком участников
    pthread_create(&threads[1], NULL, shm_history, NULL); // создание потока для работы с историей сообщений
    
    // ожидание завершения обоих потоков
    for (int i = 0; i < 2; i++) {
        pthread_join(threads[i], NULL);
    }
    
    return 0;
}

void *shm_list() {
    int LIST_SHM_SIZE = 256; // начальный размер сегмента разделяемой памяти
    
    // создание сегмента разделяемой памяти в виде файла
    int shm_list_fd = shm_open("/shm_list", O_CREAT | O_RDWR, 0666); // получаем файловый дескриптор
    if (shm_list_fd < 0) {
        perror("shm_open: не удалось создать сегмент разделяемой памяти. код: list\n");
        exit(EXIT_FAILURE);
    }
    
    // урезаем размер созданного файла
    if (ftruncate(shm_list_fd, LIST_SHM_SIZE) < 0) {
        perror("ftruncate: не удалось урезать размер сегмента разделяемой памяти. код: list\n");
        shm_unlink("/shm_list");
        exit(EXIT_FAILURE);
    }
    
    // отображение страниц в виртуальное адресное пространство программы
    struct list *shm_address = mmap(NULL, LIST_SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_list_fd, 0); // создание динамического массива в сегменте
    if (shm_address == MAP_FAILED) {
        perror("mmap: не удалось отобразить страницы сегмента разделяемой памяти в адресном пространстве программы. код: list\n");
        shm_unlink("/shm_list");
        exit(EXIT_FAILURE);
    }
    
    // затирание сегмента при повторном запуске сервера
    for(int i = 0; shm_address[i].nickname[0] != '\0'; i++) {
        shm_address[i].pid = 0;
        
        for (int j = 0; shm_address[i].nickname[j] != '\0'; j++) {
            shm_address[i].nickname[j] = '\0';
        }
    }
    
    
    // создание семафора, который обеспечивает синхронизацию между клиентом и сервером
    sem_t *sem_list = sem_open("/sem_list", O_CREAT, 0666, 0);
    if (sem_list == SEM_FAILED) {
        perror("sem_open: не удалось создать семафор sem_list. код: list\n");
        shm_unlink("/shm_list");
        munmap(shm_address, LIST_SHM_SIZE);
        exit(EXIT_FAILURE);
    }
    
    sem_init(sem_list, 1, 0);
    
    // создание семафора, который обеспечивает синхронизацию между клиентами
    sem_t *sem_list_clients = sem_open("/sem_list_clients", O_CREAT, 0666, 0);
    if (sem_list_clients == SEM_FAILED) {
        perror("sem_open: не удалось создать семафор sem_list_clients. код: list\n");
        shm_unlink("/shm_list");
        munmap(shm_address, LIST_SHM_SIZE);
        exit(EXIT_FAILURE);
    }
    
    sem_init(sem_list_clients, 1, 1);
    
    int user_counter = 0; // счётчик участников чата
    
    while (1) {
        sem_wait(sem_list);
        
        user_counter++;
        
        if (user_counter == LIST_SHM_SIZE/2) {
            LIST_SHM_SIZE *= 2;
            
            // урезаем размер созданного файла
            if (ftruncate(shm_list_fd, LIST_SHM_SIZE) < 0) {
                perror("ftruncate: не удалось урезать размер сегмента разделяемой памяти. код: list\n");
                shm_unlink("/shm_list");
                exit(EXIT_FAILURE);
            }
            
            // отображение страниц в виртуальное адресное пространство программы
            struct list *shm_address = mmap(NULL, LIST_SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_list_fd, 0); // создание динамического массива в сегменте
            if (shm_address == MAP_FAILED) {
                perror("mmap: не удалось отобразить страницы сегмента разделяемой памяти в адресном пространстве программы. код: list\n");
                shm_unlink("/shm_history");
                exit(EXIT_FAILURE);
            }
        }
    }
    
    return NULL;
}

void *shm_history() {
    int HISTORY_SHM_SIZE = 1024; // начальный размер сегмента разделяемой памяти
    
    // создание сегмента разделяемой памяти в виде файла
    int shm_history_fd = shm_open("/shm_history", O_CREAT | O_RDWR, 0666); // получаем файловый дескриптор
    if (shm_history_fd < 0) {
        perror("shm_open: не удалось создать сегмент разделяемой памяти. код: history\n");
        exit(EXIT_FAILURE);
    }
    
    // урезаем размер созданного файла
    if (ftruncate(shm_history_fd, HISTORY_SHM_SIZE) < 0) {
        perror("ftruncate: не удалось урезать размер сегмента разделяемой памяти. код: history\n");
        shm_unlink("/shm_history");
        exit(EXIT_FAILURE);
    }
    
    // отображение страниц в виртуальное адресное пространство программы
    struct history *shm_address = mmap(NULL, HISTORY_SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_history_fd, 0); // создание динамического массива в сегменте
    if (shm_address == MAP_FAILED) {
        perror("mmap: не удалось отобразить страницы сегмента разделяемой памяти в адресном пространстве программы. код: history\n");
        shm_unlink("/shm_history");
        exit(EXIT_FAILURE);
    }
    
    // затирание cегмента при повторном запуске сервера
    for (int i = 0; shm_address[i].nickname[0] != '\0'; i++) {
        for (int j = 0; shm_address[i].nickname[j] != '\0'; j++) {
            shm_address[i].nickname[j] = '\0';
        }
        
        for (int j = 0; shm_address[i].msg[j] != '\0'; j++) {
            shm_address[i].msg[j] = '\0';
        }
    }
    
    // создание семафора в виде файла, который обеспечивает синхронизацию между клиентом и сервером
    sem_t *sem_history = sem_open("/sem_history", O_CREAT, 0666, 0);
    if (sem_history == SEM_FAILED) {
        perror("sem_open: не удалось создать семафор sem_history. код: history\n");
        shm_unlink("/shm_history");
        munmap(shm_address, HISTORY_SHM_SIZE);
        exit(EXIT_FAILURE);
    }
    
    sem_init(sem_history, 1, 0);
    
    // создание семафора, который обеспечивает синхронизацию между клиентами
    sem_t *sem_history_clients = sem_open("/sem_history_clients", O_CREAT, 0666, 0);
    if (sem_history_clients == SEM_FAILED) {
        perror("sem_open: не удалось создать семафор sem_history_clients. код: history\n");
        shm_unlink("/shm_history");
        munmap(shm_address, HISTORY_SHM_SIZE);
        exit(EXIT_FAILURE);
    }
    
    sem_init(sem_history_clients, 1, 1);
    
    int msg_counter = 0; // счётчик сообщений в чате
    
    while (1) {
        sem_wait(sem_history); // ожидание нового сообщения
        
        msg_counter++; // увеличиваем число сообщений на 1
        
        if (msg_counter == HISTORY_SHM_SIZE/2) {
            
            HISTORY_SHM_SIZE *= 2; // вдвое увеличиваем размер массива
            
            // урезаем размер файла
            if (ftruncate(shm_history_fd, HISTORY_SHM_SIZE) < 0) {
                perror("ftruncate: не удалось урезать размер файла. код: history\n");
                shm_unlink("/shm_history");
                exit(EXIT_FAILURE);
            }
            
            // отображение страниц в виртуальное адресное пространство программы
            struct history *shm_address = mmap(NULL, HISTORY_SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_history_fd, 0);
            if (shm_address == MAP_FAILED) {
                perror("mmap: не удалось отобразить сегмент разделяемой памяти в программе. код: history\n");
                shm_unlink("/shm_history");
                exit(EXIT_FAILURE);
            }
        }
    }
    return NULL;
}
