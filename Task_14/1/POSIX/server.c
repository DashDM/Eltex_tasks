#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>

#define SHM_SIZE 256

int main(void) {
    /*начальные данные клиента*/
    char *msg = "Hello, Client!";
    char buff[SHM_SIZE] = {0};
    
    int shm_fd = shm_open("/my_shm", O_RDWR, 0666); // открываем файл и сохраняем его дескриптор
    if (shm_fd < 0) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
    
    /*отображение страниц в виртуальное адресное пространство процесса (программы)*/
    char *shm_address = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_address == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }
    
    /*открытие семафора*/
    sem_t *sem = sem_open("/my_sem", 0);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        munmap(shm_address, SHM_SIZE);
        shm_unlink("/my_shm");
        exit(EXIT_FAILURE);
    }
    
    /*чтение сообщения из сегмента разделяемой памяти*/
    int i = 0;
    for (i; shm_address[i] != '\0'; i++) {
        buff[i] = shm_address[i];
    }
    
    printf("Received: %s\n", buff);
    
    /*запись сообщения в сегмент разделяемой памяти*/
    i = 0;
    for(i; msg[i] != '\0'; i++) {
        shm_address[i] = msg[i];
    }
    shm_address[i] = '\0';
    
    // разблокировали процесс
    if (sem_post(sem) < 0) {
        perror("sem_post");
        exit(EXIT_FAILURE);
    }
    
    /*очистка*/
    // удаляем отображение семафора в виртуальном адресное пространстве программы
    if (sem_close(sem) < 0) {
        perror("sem_close");
    }
    
    // удаляем отображение сегмента разделяемой памяти из текущего процесса
    if ( munmap(shm_address, SHM_SIZE) < 0) {
        perror("munmap");
    }
    
    return 0;
}

