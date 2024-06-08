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
    struct msgbuf client_msg = {.mtype = 2, .mtext = "qq, server!"};
    struct msgbuf server_msg;
    
    key_t key = ftok("client.c", 5); // номер очереди
    
    int qid = msgget(key, IPC_CREAT | 0666); // дескриптор очереди
    if (qid < 0) {
        perror("ошибка при создании очереди");
        exit(EXIT_FAILURE);
    } else {
        printf("Очередь успешно создана, её ключ: %d, её дескриптор: %d\n", key, qid);
    }
    
    if (msgsnd(qid, &client_msg, sizeof(client_msg.mtext), 0) < 0) {
        perror("ошибка при записи сообщения в очередь\n");
        exit(EXIT_FAILURE);
    } else {
        printf("Сообщение успешно записано в очередь: %s, размер буфера: %ld\n", client_msg.mtext, sizeof(client_msg.mtext));
    }
    
    if (msgrcv(qid, &server_msg, sizeof(server_msg.mtext), 1, 0) < 0) {
        perror("ошибка при чтении сообщения\n");
        exit(EXIT_FAILURE);
    } else {
        printf("клиентом получен ответ --> %s\n", server_msg.mtext);
    }
    
    if(msgctl(qid, IPC_RMID, NULL) < 0) {
        perror("ошибка при удалении очереди\n");
        exit(EXIT_FAILURE);
    }
    
    return 0;
}
