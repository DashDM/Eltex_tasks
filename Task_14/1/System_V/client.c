#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define SHM_SIZE 256

int main(void) {
    /*начальные данные клиента*/
    char *msg = "Hello, Server!";
    char buff[SHM_SIZE] = {0};
    
    /*сегмент разделяемой памяти*/
    key_t shm_key = ftok("client.c", 5);
    if (shm_key < 0) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }
    
    int shm_id= shmget(shm_key, SHM_SIZE, IPC_CREAT | 0666); // создаём сегмент разделяемой памяти
    if (shm_id < 0) {
        perror("creat shmget");
        exit(EXIT_FAILURE);
    }
    
    char *shm_address = shmat(shm_id, NULL, 0); // отображение сегмента разделяемой памяти в процесс
    if (shm_address == (void*)-1) {
        perror("shmat");
        shmctl(shm_id, IPC_RMID, NULL);
        exit(EXIT_FAILURE);
    }
    
    /*семафор*/
    key_t sem_key = ftok("client.c", 7);
    if (sem_key < -1) {
        perror("ftok");
        shmdt(shm_address);
        shmctl(shm_id, IPC_RMID, NULL);
        exit(EXIT_FAILURE);
    }
    
    int sem_id = semget(sem_key, 1, IPC_CREAT | 0666); // изначально sem_op = 0
    if (sem_id < 0) {
        perror("semget");
        shmdt(shm_address);
        shmctl(shm_id, IPC_RMID, NULL);
        exit(EXIT_FAILURE);
    }
    
    struct sembuf lock = {0, -1, 0};
    
    /*отправка сообщения от клиента к серверу*/
    int i = 0;
    for (i; msg[i] != '\0'; i++) {
        shm_address[i] = msg[i];
    }
    shm_address[i] = '\0';
    
    // клиент блокируется (значение семафора становится sem_op = -1)
    if (semop(sem_id, &lock, 1) < 0) {
        perror("semop");
        shmdt(shm_address);
        shmctl(shm_id, IPC_RMID, NULL);
        semctl(sem_id, IPC_RMID, 0);
        exit(EXIT_FAILURE);
    }
    
    /*чтение ответа из сегмента разделяемой памяти*/
    i = 0;
    for (i; shm_address[i] != '\0'; i++) {
        buff[i] = shm_address[i];
    }
    
    printf("Received: %s\n", buff);
    
    // отсоединение от сегмента разделяемой памяти
    if (shmdt(shm_address) < 0) {
        perror("shmdt");
    }
    
    // полное удаление физ страницы сегмента рп на уровне ядра
    if (shmctl(shm_id, IPC_RMID, NULL) < 0) {
        perror("shmctl");
    }
    
    // полное удаление семафора на уровне ядра
    if (semctl(sem_id, IPC_RMID, 0) < 0) {
        perror("semctl");
    }
    
    return 0;
}
