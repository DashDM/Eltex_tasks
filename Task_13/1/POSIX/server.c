#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#define msg_size 256

int main(void) {
    char message[] = "oh no, it's u again..";
    char buffer[msg_size];
    unsigned int prior = 2; // приоритет для чтения из очереди
    // определение атрибутов (параметров) очереди [.флаги, .макс число сообщений в очереди, .макс размер каждого сообщения, и, разумеется, .curmsgs]
    struct mq_attr attr = {
        .mq_flags = 0,
        .mq_maxmsg = 10,
        .mq_msgsize = sizeof(message) + 1,
        .mq_curmsgs = 0
    };
    
    mqd_t mq_client_to_server = mq_open("/client_queue", O_RDONLY);
    if (mq_client_to_server < 0) {
        perror("ошибка при открытии очереди");
        exit(EXIT_FAILURE);
    } else {
        printf("Очередь успешно открыта, её дескриптор: %d\n", mq_client_to_server);
    }
    
    if (mq_receive(mq_client_to_server, buffer, sizeof(buffer), &prior) < 0) {
        perror("ошибка при чтении сообщения из очереди\n");
        exit(EXIT_FAILURE);
    } else {
        printf("server получил --> %s\n", buffer);
    }
    mq_close(mq_client_to_server);
    unlink("/client_queue");
    
    mqd_t mq_server_to_client = mq_open("/server_queue", O_CREAT | O_WRONLY, 0666, &attr);
    if (mq_server_to_client < 0) {
        perror("ошибка при создании очереди");
        exit(EXIT_FAILURE);
    } else {
        printf("Очередь успешно создана, её дескриптор: %d\n", mq_server_to_client);
    }
    
    if (mq_send(mq_server_to_client, message, sizeof(message), 1) < 0) {
        perror("ошибка при записи сообщения в очередь\n");
        exit(EXIT_FAILURE);
    } else {
        printf("Сообщение успешно записано в очередь\n");
    }
    mq_close(mq_server_to_client);
    
    return 0;
}

