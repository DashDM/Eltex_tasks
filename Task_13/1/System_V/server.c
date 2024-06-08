#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define msg_size 256

struct msgbuf {
    long mtype;
    char mtext[msg_size];
};

int main(void) {
    struct msgbuf server_msg = {.mtype = 1, .mtext = "Hello, client!"};
    struct msgbuf client_msg;
    
    key_t key = ftok("client.c", 5); // номер очереди
    
    int qid = msgget(key, 0); // открываем существующую очередь
    if (qid < 0) {
        perror("ошибка при открытии очереди\n");
        exit(EXIT_FAILURE);
    } else {
        printf("Очередь успешно открыта, её ключ: %d, её дескриптор: %d\n", key, qid);
    }
    
    if (msgrcv(qid, &client_msg, sizeof(client_msg.mtext), 2, 0) < 0) {
        perror("ошибка при чтении сообщения\n");
        exit(EXIT_FAILURE);
    } else {
        printf("сервером получено --> %s\n", client_msg.mtext);
    }
    
    if (msgsnd(qid, &server_msg, sizeof(server_msg.mtext), 0) < 0) {
        perror("ошибка при записи сообщения в очередь\n");
        exit(EXIT_FAILURE);
    }
    
    return 0;
}
