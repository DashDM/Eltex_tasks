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
/*
void *shm_list() {
    long LIST_SHM_SIZE = 1024; // начальный размер сегмента разделяемой памяти
    
    // --------------------------------------------------
    // создание сегмента разделяемой памяти в виде файла
    int shm_list_fd = shm_open("/shm_list", O_CREAT | O_RDWR, 0666); // получаем файловый дескриптор
    if (shm_list_fd < 0) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
    
    // урезаем размер файла
    if(ftruncate(shm_list_fd, LIST_SHM_SIZE) < 0) {
        perror("ftruncate");
        shm_unlink("/shm_list");
        exit(EXIT_FAILURE);
    }
    
    // отображение страниц в виртуальное адресное пространство процесса
    void *shm_address = mmap(NULL, LIST_SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_list_fd, 0);
    if (shm_address == MAP_FAILED) {
        perror("mmap");
        shm_unlink("/shm_list");
        exit(EXIT_FAILURE);
    }
    // --------------------------------------------------
    
    //инициализация динамического массива в сегменте разделяемой памяти для списка участников
    struct list *users = shm_address;
    
    //зануляем сегмент разделяемой памяти
    for (int i = 0; users[i].nickname[0] != '\0'; i++) {
        for (int j  = 0; users[i].nickname[j] != '\0'; j++) {
            users[i].nickname[j] = '\0';
        }
    }
    
    //создание семафора (синхронизация между клиентом и сервером)
    sem_t *sem_server_block = sem_open("/sem_list_server", O_CREAT | O_RDWR, 0666, 0);
    if (sem_server_block == SEM_FAILED) {
        perror("sem_open");
        munmap(shm_address, LIST_SHM_SIZE);
        shm_unlink("/shm_list");
        exit(EXIT_FAILURE);
    }
    
    //создание семафора (синхронизация между клиентами)
    sem_t *sem_client = sem_open("/sem_client", O_CREAT, 0666, 1);
    if (sem_client == SEM_FAILED) {
        perror("sem_open");
        munmap(shm_address, LIST_SHM_SIZE);
        shm_unlink("/shm_list");
        exit(EXIT_FAILURE);
    }
    
    sem_post(sem_client);
    
    // --------------------------------------------------
    int users_count = 0; // счётчик пользователей
    
    while (1) {
        sem_wait(sem_server_block); // блокируется до тех пор, пока не подключится новый пользователь
        
        users_count++;
        
        // проверка: нужно ли увеличить размер динамического массива
        if (users_count == LIST_SHM_SIZE/2) {
            LIST_SHM_SIZE *= 2;
            
            if(ftruncate(shm_list_fd, LIST_SHM_SIZE) < 0) {
                perror("ftruncate");
                shm_unlink("/shm_list");
                exit(EXIT_FAILURE);
            }
            
            //отображение страниц в виртуальное адресное пространство процесса
            void *shm_address = mmap(NULL, LIST_SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_list_fd, 0);
            if (shm_address == MAP_FAILED) {
                perror("mmap");
                shm_unlink("/shm_list");
                exit(EXIT_FAILURE);
            }
        }
    }
    // --------------------------------------------------
    
    return NULL;
}
*?

/*
void *shm_list() {
    long LIST_SHM_SIZE = 1024; // начальный размер сегмента разделяемой памяти
    
    // --------------------------------------------------
    //создание сегмента разделяемой памяти в виде файла
    int shm_list_fd = shm_open("/shm_list", O_CREAT | O_RDWR, 0666);
    if (shm_list_fd < 0) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
    
    //урезаем размер файла (для себя: скорее всего нужно будет перезаписывать массив в сегмент, поэтому лучше хранить инфу на сервере. Хотя хз, мб получится увеличить с помощью ftruncate() и снова отобразить с помощью mmap(). Хотя на лекции говорили, что фиг нам, а не увеличить размер с помощью ftruncate())
    if(ftruncate(shm_list_fd, LIST_SHM_SIZE) < 0) {
        perror("ftruncate");
        shm_unlink("/shm_list");
        exit(EXIT_FAILURE);
    }
    
    // отображение страниц в виртуальное адресное пространство процесса
    void *shm_address = mmap(NULL, LIST_SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_list_fd, 0);
    if (shm_address == MAP_FAILED) {
        perror("mmap");
        shm_unlink("/shm_list");
        exit(EXIT_FAILURE);
    }
    // --------------------------------------------------
    
    //инициализация динамического массива для списка участников
    struct list *users = shm_address;
    users[0].pid  = 0; 
    users[0].nickname[0] = '\0';
    // --------------------------------------------------
    
    //создание семафора (сначала инфу прочитает сервер)
    sem_t *sem_list = sem_open("/sem_list", O_CREAT | O_RDWR, 0666, 0); // значение семафора при инициализации = 0
    if (sem_list == SEM_FAILED) {
        perror("sem_open");
        munmap(shm_address, LIST_SHM_SIZE);
        shm_unlink("/shm_list");
        exit(EXIT_FAILURE);
    }
    
    
    int value;
    
    // --------------------------------------------------
    int users_count = 0; // счётчик пользователей
    
    while (1) {
        sem_wait(sem_list); // блокируется до тех пор, пока не подключится новый пользователь
        
        for (int i = 0; users[i].nickname[0] != '\0'; i++) {
            users_count++;
        }
        
        if (users_count == LIST_SHM_SIZE/2) {
            LIST_SHM_SIZE *= 2;
            
            if(ftruncate(shm_list_fd, LIST_SHM_SIZE) < 0) {
                perror("ftruncate");
                shm_unlink("/shm_list");
                exit(EXIT_FAILURE);
            }
            
            //отображение страниц в виртуальное адресное пространство процесса
            void *shm_address = mmap(NULL, LIST_SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_list_fd, 0);
            if (shm_address == MAP_FAILED) {
                perror("mmap");
                shm_unlink("/shm_list");
                exit(EXIT_FAILURE);
            }
        }
    }
    // --------------------------------------------------
    
    return NULL;
}

void *shm_history() {
    long HISTORY_SHM_SIZE = 1024; // начальный размер сегмента разделяемой памяти
    
    // --------------------------------------------------
    //создание сегмента разделяемой памяти в виде файла
    int shm_history_fd = shm_open("/shm_history", O_CREAT | O_RDWR, 0666);
    if (shm_history_fd < 0) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
    
    //урезаем размер файла (для себя: скорее всего нужно будет пересоздавать и перезаписывать массив в сегмент, поэтому лучше хранить инфу на сервере. Хотя хз)
    if(ftruncate(shm_history_fd, HISTORY_SHM_SIZE) < 0) {
        perror("ftruncate");
        shm_unlink("/shm_history");
        exit(EXIT_FAILURE);
    }
    
    //отображение страниц в виртуальное адресное пространство процесса
    void *shm_address = mmap(NULL, HISTORY_SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_history_fd, 0);
    if (shm_address == MAP_FAILED) {
        perror("mmap");
        shm_unlink("/shm_history");
        exit(EXIT_FAILURE);
    }
    // --------------------------------------------------
    
    //инициализация динамического массива для списка участников
    struct history *chat = shm_address;
    chat[0].nickname[0] = '\0';
    chat[0].msg[0] = '\0';
    // --------------------------------------------------
    
    //создание семафора
    sem_t *sem_history = sem_open("/sem_history", O_CREAT | O_RDWR, 0666, 0); // значение семафора при инициализации = 0
    if (sem_history == SEM_FAILED) {
        perror("sem_open");
        munmap(shm_address, HISTORY_SHM_SIZE);
        shm_unlink("/shm_history");
        exit(EXIT_FAILURE);
    }
    
    // --------------------------------------------------
    int msgs_count = 0; // счётчик пользователей
    
    while (1) {
        sem_wait(sem_history); // значение семафора уменьшается на 1 => переходит в ожидание
        
        for (int i = 0; chat[i].msg[0] != '\0'; i++) {
            msgs_count++;
        }
        
        if (msgs_count == HISTORY_SHM_SIZE/2) {
            HISTORY_SHM_SIZE *= 2;
            
            if(ftruncate(shm_history_fd, HISTORY_SHM_SIZE) < 0) {
                perror("ftruncate");
                shm_unlink("/shm_history");
                exit(EXIT_FAILURE);
            }
            
            //отображение страниц в виртуальное адресное пространство процесса
            void *shm_address = mmap(NULL, HISTORY_SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_history_fd, 0);
            if (shm_address == MAP_FAILED) {
                perror("mmap");
                shm_unlink("/shm_history");
                exit(EXIT_FAILURE);
            }
        }
    }
    // --------------------------------------------------
    
    
    return NULL;
}
*/




/*
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
*/
