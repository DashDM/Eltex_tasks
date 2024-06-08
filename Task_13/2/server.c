#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAX_MSG 256
#define MAX_NICKNAME 10
#define MAX_CHAT_HISTORY 20
#define SERVER_TYPE 1

// структура | используется как кадр для ЧТЕНИЯ одного сообщения из очереди
struct user_data {
    long type; // приоритет для чтения из очереди
    char nickname[MAX_NICKNAME]; // ник пользователя
    char msg[MAX_MSG]; // сообщение, что он отправил
    int uid; // идентификатор пользователя | используется в качестве приоритета для передачи "server -> client"
    short status; // статус пользователя (0 - оффлайн, 1 - онлайн, 2 - вводит сообщение) | используется для выхода и цветовой пары в списке участников
};

// структра | используется как кадр для ЗАПИСИ одного сообщения в очередь
struct chat_data {
    long type; // приоритет для записи/чтения
    char nickname[MAX_NICKNAME]; // история сообщений (кто написал) | ник пользователя из списка участников чата
    char msg[MAX_MSG]; // история сообщений (что написал)
    int list_size; // сколько всего участников | используется для сброса координаты Y при выводе на экран
    int history_size; // сколько всего сообщений | используется для сброса координаты Y при выводе на экран
    short status; // статус пользователя (0 - оффлайн, 1 - онлайн, 2 - вводит сообщение) | используется для выхода и цветовой пары в списке участников
};

// структура для массива - хранилища истории сообщений
struct history_data {
    char h_nickname[MAX_NICKNAME]; // кто написал
    char h_msg[MAX_MSG]; // что написал
};

// структура для массива - хранилища списка участников
struct list_data {
    char l_nickname[MAX_NICKNAME]; // ник участника
    int l_uid; // его идентификатор
    short l_status; // его статус
};


void change_history_size(struct history_data **history, int *HISTORY_SIZE); // изменение размера динамического массива истории сообщений

void change_list_size(struct list_data **list, int *LIST_SIZE); // изменение размера динамического массива списка участников чата

void list_sending(int qid, int LIST_COUNTER, struct list_data *list); // отправка никнеймов из списка участников в очередь сообщений

void history_sending(int qid, int HISTORY_COUNTER, struct history_data *history, int LIST_COUNTER, struct list_data *list); //  отправка истории чата в очередь сообщений

int main(void) {
    // ---создание очереди сообщений---
    key_t key = ftok("client.c", 5); // генерируем ключ
    int qid = msgget(key, IPC_CREAT | 0666); // создаём очередь сообщений с максимальными правами для всех
    if (qid < 0) {
        perror("qid");
        exit(EXIT_FAILURE);
    }
    // --------------------------------
    
    // ---инициализация начальных массивов данных---
    int HISTORY_SIZE = 50; // начальный размер динамического массива history
    int HISTORY_COUNTER = 0; // кол-во сообщений в чате на данный момент
    
    struct history_data *history = calloc(HISTORY_SIZE, sizeof(struct history_data)); // хранилище истории сообщений
    if (history == NULL) {
        perror("history -> calloc");
        exit(EXIT_FAILURE);
    }
    
    int LIST_SIZE = 10; // начальный размер динамического массива list
    int LIST_COUNTER = 0; // кол-во сообщений в чате на данный момент
    
    struct list_data *list = calloc(LIST_SIZE, sizeof(struct list_data)); // хранилище списка участников чата
    if (list == NULL) {
        perror("list -> calloc");
        exit(EXIT_FAILURE);
    }
    // ---------------------------------------------
    
    // ---инициализация структуры для чтения---
    struct user_data user = {
        .type = 0,
        .nickname = {0},
        .msg = {0},
        .uid = 0,
        .status = 0
    };
    // ----------------------------------------
    int length = 0; // счётчик для подсчёта длины строки
    
    // ---выполнение приёма&передачи---
    while (1) {
        if (msgrcv(qid, (void *)&user, sizeof(struct user_data), SERVER_TYPE, 0) < 0) {
            perror("msgrcv -> user");
            exit(EXIT_FAILURE);
        } else {
            switch (user.status) {
                case 0:
                    // ---пользователь отключился---
                    // находим пользователя с соответствующем идентификатором и заменяем его статус на 0
                    for (int i = 0; i < LIST_COUNTER; i++) {
                        if (user.uid == list[i].l_uid) {
                            list[i].l_status = 0;
                            break;
                        }
                    }
                    
                    list_sending(qid, LIST_COUNTER, list); // отправка списка участников в очередь сообщений
                    
                    break;
                case 1:
                    // ---подключился новый пользователь---
                    // посимвольно копируем никнейм пользователя в соответствующее поле массива list
                    while(user.nickname[length] != '\0') {
                        list[LIST_COUNTER].l_nickname[length] = user.nickname[length];
                        length++;
                    }
                    
                    list[LIST_COUNTER].l_uid = user.uid; // копируем идентификатор пользователя в соответствующее поле массива list
                    list[LIST_COUNTER].l_status = user.status; // копируем статус пользователя в соответствующее поле массива list
                    
                    LIST_COUNTER++; // увеличиваем счётчик участников чата на единицу
                    
                    // проверка на необходимость увеличить размер хранилища списка участников
                    if (LIST_COUNTER == LIST_SIZE/2) {
                        change_list_size(&list, &LIST_SIZE);
                    }
                    
                    list_sending(qid, LIST_COUNTER, list); // отправка списка участников в очередь сообщений
                    
                    history_sending(qid, HISTORY_COUNTER, history, LIST_COUNTER, list); // отправка истории чата в очередь сообщений
                    
                    length = 0; // обнуляем счётчик
                    
                    break;
                case 2:
                    // ---пользователь отправил сообщение---
                    // посимвольно копируем никнейм пользователя в соответствующее поле массива history
                    while(user.nickname[length] != '\0') {
                        history[HISTORY_COUNTER].h_nickname[length] = user.nickname[length];
                        length++;
                    }
                    
                    length = 0; // обнуляем счётчик
                    
                    // посимвольно копируем сообщение, которое отправил пользователь в соответствующее поле массива history
                    while(user.msg[length] != '\0') {
                        history[HISTORY_COUNTER].h_msg[length] = user.msg[length];
                        length++;
                    }
                    
                    HISTORY_COUNTER++; // увеличиваем счётчик кол-ва сообщений на единицу
                    
                    // проверка на необходимость увеличить размер хранилища истории сообщений
                    if (HISTORY_COUNTER == HISTORY_SIZE/2) {
                        change_history_size(&history, &HISTORY_SIZE);
                    }
                    
                    history_sending(qid, HISTORY_COUNTER, history, LIST_COUNTER, list); // отправка истории чата в очередь сообщений
                    
                    length = 0;
                    
                    break;
            }
        }
    }
    
    free(history);
    free(list);
    
    return 0;
}

void history_sending(int qid, int HISTORY_COUNTER, struct history_data *history, int LIST_COUNTER, struct list_data *list) {
    int length = 0; // счётчик для подсчёта длины строки
    
    // вдоль массива list
    for (int i = 0; i < HISTORY_COUNTER; i++) {
        // инициализируем структуру для отправки списка участников чата
        struct chat_data send = {
            .type = 0,
            .nickname = {0},
            .msg = {0},
            .list_size = 0,
            .history_size = HISTORY_COUNTER,
            .status = 2 // 2 для сообщений
        };
        
        // посимвольно копируем никнейм из массива history в соответствующее поле структуры send
        while(history[i].h_nickname[length] != '\0') {
            send.nickname[length] = history[i].h_nickname[length];
            length++;
        }
        send.nickname[length] = '\0'; // добавляем нуль-терминатор
        
        length = 0; // обнуляем счётчик
        
        // посимвольно копируем сообщение из массива history в соответствующее поле структуры send
        while(history[i].h_msg[length] != '\0') {
            send.msg[length] = history[i].h_msg[length];
            length++;
        }
        send.msg[length] = '\0'; // добавляем нуль-терминатор
        
        length = 0;
        
        // рассылаем никнейм и статус пользователя каждому подключенному участнику чата
        for (int j = 0; j < LIST_COUNTER; j++) {
            if (list[j].l_status != 0) {
                send.type = list[j].l_uid;
                // отправляем объект в очередь сообщений
                if (msgsnd(qid, (void *)&send, sizeof(struct chat_data), 0) < 0) {
                    perror("msgsnd");
                    exit(EXIT_FAILURE);
                }
            }
        }
    }
}

void list_sending(int qid, int LIST_COUNTER, struct list_data *list) {
    int length = 0; // счётчик для подсчёта длины строки
    
    // вдоль массива list
    for (int i = 0; i < LIST_COUNTER; i++) {
        // инициализируем структуру для отправки списка участников чата
        struct chat_data send = {
            .type = 0,
            .nickname = {0},
            .msg = {0},
            .list_size = LIST_COUNTER,
            .history_size = 0,
            .status = list[i].l_status // будет либо 0, либо 1
        };
        
        // посимвольно копируем никнейм из массива list в соответствующее поле структуры send
        while(list[i].l_nickname[length] != '\0') {
            send.nickname[length] = list[i].l_nickname[length];
            length++;
        }
        send.nickname[length] = '\0'; // добавляем нуль-терминатор
        
        length = 0;
        
        // рассылаем никнейм и статус пользователя каждому подключенному участнику чата
        for (int j = 0; j < LIST_COUNTER; j++) {
            if (list[j].l_status != 0) {
                send.type = list[j].l_uid;
                
                // отправляем объект в очередь сообщений
                if (msgsnd(qid, (void *)&send, sizeof(struct chat_data), 0) < 0) {
                    perror("msgsnd");
                    exit(EXIT_FAILURE);
                }
            }
        }
    }
}

void change_history_size(struct history_data **history, int *HISTORY_SIZE) {
    *HISTORY_SIZE *= 2;
    *history = realloc(*history, *HISTORY_SIZE * sizeof(struct history_data));
}

void change_list_size(struct list_data **list, int *LIST_SIZE) {
    *LIST_SIZE *= 2;
    *list = realloc(*list, *LIST_SIZE * sizeof(struct list_data));
}
