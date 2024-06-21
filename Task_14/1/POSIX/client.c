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
    char *msg = "Hello, Server!";
    char buff[SHM_SIZE] = {0};
    
    /*создаём сегмент разделяемой памяти*/
    int shm_fd = shm_open("/my_shm", O_CREAT | O_RDWR, 0666);
    if (shm_fd < 0) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
    
    // усекаем размер файла
    if (ftruncate(shm_fd, SHM_SIZE) < 0) {
        perror("ftruncate");
        shm_unlink("/my_shm");
        exit(EXIT_FAILURE);
    }
    
    /*отображение страниц в виртуальное адресное пространство процесса (программы)*/
    char *shm_address = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_address == MAP_FAILED) {
        perror("mmap");
        shm_unlink("/my_shm");
        exit(EXIT_FAILURE);
    }
    
    /*создание семафора*/
    sem_t *sem = sem_open("/my_sem", O_CREAT | O_RDWR, 0666, 0);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        munmap(shm_address, SHM_SIZE);
        shm_unlink("/my_shm");
        exit(EXIT_FAILURE);
    }
    
    /*запись сообщения в сегмент разделяемой памяти*/
    int i= 0;
    for (i; msg[i] != '\0'; i++) {
        shm_address[i] = msg[i];
    }
    shm_address[i] = '\0';
    
    // заблокировали процесс
    if(sem_wait(sem) < 0) {
        perror("sem_wait");
        sem_close(sem);
        sem_unlink("/my_sem");
        munmap(shm_address, SHM_SIZE);
        shm_unlink("/my_shm");
        exit(EXIT_FAILURE);
    }
    
    /*чтение сообщения из сегмента разделяемой памяти*/
    i = 0;
    for (i; shm_address[i] != '\0'; i++) {
        buff[i] = shm_address[i];
    }
    
    printf("Received: %s\n", buff);
    
    /*очистка*/
    // удаляем отображение семафора в виртуальном адресное пространстве программы
    if (sem_close(sem) < 0) {
        perror("sem_close");
    }
    
    // удаляем семафор на уровне ядра
    if (sem_unlink("/my_sem") < 0) {
        perror("sem_unlink");
    }
    
    // удаляем отображение сегмента разделяемой памяти из текущего процесса
    if ( munmap(shm_address, SHM_SIZE) < 0) {
        perror("munmap");
    }
    
    // полностью удаляем сегмент разделяемой памяти
    if (shm_unlink("/my_shm") < 0) {
        perror("shm_unlink");
    }
    
    return 0;
}

