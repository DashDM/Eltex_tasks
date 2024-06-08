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
    char message[] = "Hi, Server, it's me - Mario!"; // сообщение для записи в очередь
    char buffer[msg_size]; // буфер для чтения из очереди
    unsigned int prior = 1; // приоритет для чтения из очереди
    // определение атрибутов (параметров) очереди [.флаги, .макс число сообщений в очереди, .макс размер каждого сообщения, и, разумеется, .curmsgs]
    struct mq_attr attr = {
        .mq_flags = 0,
        .mq_maxmsg = 10,
        .mq_msgsize = sizeof(message) + 1,
        .mq_curmsgs = 0
    };
    
    // создаём очередь сообщений для отправки: клиент --> сервер
    mqd_t mq_client_to_server = mq_open("/client_queue", O_CREAT | O_WRONLY, 0666, &attr);
    if (mq_client_to_server < 0) {
        perror("ошибка при создании очереди");
        exit(EXIT_FAILURE);
    } else {
        printf("Очередь успешно создана, её дескриптор: %d\n", mq_client_to_server);
    }
    
    if (mq_send(mq_client_to_server, message, sizeof(message), 2) < 0) {
        perror("ошибка при записи сообщения в очередь\n");
        exit(EXIT_FAILURE);
    } else {
        printf("Сообщение успешно записано в очередь\n");
    }
    mq_close(mq_client_to_server);
    
    // создаём очередь сообщений для приёма: сервер --> клиент
    mqd_t mq_server_to_client = mq_open("/server_queue", O_RDONLY);
    if (mq_server_to_client < 0) {
        perror("ошибка при открытии очереди");
        exit(EXIT_FAILURE);
    } else {
        printf("Очередь успешно открыта, её дескриптор: %d\n", mq_server_to_client);
    }
    
    if (mq_receive(mq_server_to_client, buffer, sizeof(buffer), &prior) < 0) {
        perror("ошибка при чтении сообщения из очереди\n");
        exit(EXIT_FAILURE);
    } else {
        printf("server получил --> %s\n", buffer);
    }
    mq_close(mq_server_to_client);
    unlink("/server_queue");
    
    return 0;
}

