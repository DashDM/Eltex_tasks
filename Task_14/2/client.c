#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <ncurses.h>

#define MAX_NICKNAME 10
#define MAX_MSG 256

volatile bool _exit_ = false;

pthread_mutex_t wnd_mutex = PTHREAD_MUTEX_INITIALIZER; // mutex, чтобы окна создавались поочерёдно

struct list {
    int pid;
    char nickname[MAX_NICKNAME];
};

struct history {
    char nickname[MAX_NICKNAME];
    char msg[MAX_MSG];
};

struct position_wnd {
    int width; // ширина
    int height; // высота
    int x; // координата по оси х
    int y; // координата по оси у
};

void *shm_list(void *args); // ---вывод спсика участников на экран---
void *shm_history(); // ---вывод истории сообщений на экран---
void *shm_enter(void *user); // ввод сообщений

void exam_cmd(char *msg, int length);

WINDOW *create_window(struct position_wnd param); // создание окна

int main(void) {
    // инициализация структуры пользователя
    struct list user = {
        .pid = getpid(), 
        .nickname = {0}
    };
    
    // ввод никнейма
    printf("Enter nickname: ");
    fgets(user.nickname, MAX_NICKNAME, stdin);
    
    int length = 0;
    if (user.nickname[length] != '\0') {
        while (user.nickname[length] != '\0') {
            if (user.nickname[length] == '\n') {
                user.nickname[length] = '\0';
                break;
            }
            length++;
        }
    }
    
    // ---инициализируем структуру данных библиотеки ncurses.h и прочие базовые штучки---
    initscr();
    clear();
    noecho();
    curs_set(0);
    cbreak();
    
    // определяем цветовую пару для фона
    start_color();
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    bkgd(COLOR_PAIR(1));
    refresh();
    
    pthread_t threads[3]; // объявление массива потоков
    
    pthread_create(&threads[0], NULL, shm_list, &user); // создаём поток для списка участников
    pthread_create(&threads[1], NULL, shm_history, NULL); // создаём поток для чтения сообщений
    pthread_create(&threads[2], NULL, shm_enter, &user); // создаём поток для записи сообщений
    
    // ожидание закрытия каждого потока
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }
    
    endwin();
    
    return 0;
}

void *shm_list(void* args) {
    // ---создание правого окна---
    // находим параметры экрана
    int max_height, max_width;
    getmaxyx(stdscr, max_height, max_width);
    
    // задаём параметры окна
    struct position_wnd pos_right = {.width = max_height - 5, .height = max_width/2 - max_width/3 - 1, .x = 0, .y = max_width/2 + max_width/3 + 1};
    
    // объявляем цветовую пару
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    
    // создаём правое окно (список участников)
    WINDOW *right_wnd = create_window(pos_right);
    if (right_wnd == NULL) {
        perror("right_wnd");
        exit(EXIT_FAILURE);
    }
    wrefresh(right_wnd);
    
    struct list user = *(struct list *)args;
    
    // открытие сегмента разделяемой памяти
    int shm_list_fd = shm_open("/shm_list", O_RDWR, 0666);
    if (shm_list_fd < 0) {
        perror("shm_open: не удалось открыть сегмент разделяемой памяти. код: list\n");
        exit(EXIT_FAILURE);
    }
    
    // получаем статистику сегмента разделяемой памяти
    struct stat shm_stat;
    if (fstat(shm_list_fd, &shm_stat) < 0) {
        perror("fstat: не удалось получить информацию о сегменте. код: list.\n");
        shm_unlink("/shm_list");
        exit(EXIT_FAILURE);
    }
    
    // отображение страниц в виртуальное адресное пространство программы
    struct list *shm_address = mmap(NULL, shm_stat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_list_fd, 0); // создание динамического массива в сегменте
    if (shm_address == MAP_FAILED) {
        perror("mmap: не удалось отобразить страницы в виртуальное адресное пространство программы. код: list\n");
        shm_unlink("/shm_list");
        exit(EXIT_FAILURE);
    }
    
    // открытие семафора, который обеспечивает синхронизацию между клиентом и сервером
    sem_t *sem_list = sem_open("/sem_list", 0);
    if (sem_list == SEM_FAILED) {
        perror("sem_open: не удалось создать семафор sem_list. код: list\n");
        shm_unlink("/shm_list");
        munmap(shm_address, shm_stat.st_size);
        exit(EXIT_FAILURE);
    }
    
    // открытие семафора, который обеспечивает синхронизацию между клиентами
    sem_t *sem_list_clients = sem_open("/sem_list_clients", 0);
    if (sem_list_clients == SEM_FAILED) {
        perror("sem_open: не удалось создать семафор sem_list_clients. код: list\n");
        shm_unlink("/shm_list");
        munmap(shm_address, shm_stat.st_size);
        exit(EXIT_FAILURE);
    }
    
    int users_counter = 0; // счётчик числа участников чата
    int users_counter_memory = 0; // предыдущее значение счётчика
    
    sem_wait(sem_list_clients); // чтобы клиенты друг друга не перезаписывали, устанавливаем синхронизацию
    
    // находим свободное место в сегменте
    for (int i = 0; shm_address[users_counter].nickname[0] != '\0'; i++) {
        users_counter++;
    }
    
    // копируем никнейм пользователя в сегмент
    for (int i = 0; user.nickname[i] != '\0'; i++) {
        shm_address[users_counter].nickname[i] = user.nickname[i];
    }
    shm_address[users_counter].pid = user.pid; // копируем идентификатор пользователя в сегмент
    
    sem_post(sem_list); // разблокируем серверу возможность чтения из сегмента (1 новый пользователь = 1 такт)
    
    sem_post(sem_list_clients); // выход из критической зоны
    
    users_counter_memory = users_counter; // запоминаем предыдущее значение числа участников
    
    users_counter = 0; // обнуляем счётчик
    
    while (!_exit_) {
        
        while (shm_address[users_counter].nickname[0] != '\0') {
            users_counter++;
        }
        
        if (users_counter > users_counter_memory) {
            users_counter_memory = users_counter; // запоминаем новое значение
            
            int y_right = 0;
            
            // выводим ники всех участников
            for (int i = 0; shm_address[i].nickname[0] != '\0'; i++) {
                wmove(right_wnd, ++y_right, 1);
                wattron(right_wnd, COLOR_PAIR(2));
                wprintw(right_wnd, "%s", shm_address[i].nickname);
                wrefresh(right_wnd);
            }
            
            if (users_counter == shm_stat.st_size/2) {
                
                // получаем статистику сегмента разделяемой памяти
                struct stat shm_stat;
                if (fstat(shm_list_fd, &shm_stat) < 0) {
                    perror("fstat: не удалось получить информацию о сегменте. код: list.\n");
                    shm_unlink("/shm_list");
                    exit(EXIT_FAILURE);
                }
                
                // отображение страниц в виртуальное адресное пространство программы
                struct list *shm_address = mmap(NULL, shm_stat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_list_fd, 0); // создание динамического массива в сегменте
                if (shm_address == MAP_FAILED) {
                    perror("mmap: не удалось отобразить страницы в виртуальное адресное пространство программы. код: list\n");
                    shm_unlink("/shm_list");
                    exit(EXIT_FAILURE);
                }
            }
        }
        
        users_counter = 0; // обнуляем счётчик
    }
    
    /*очистка*/
    // удаляем отображение семафора в виртуальном адресное пространстве программы
    if (sem_close(sem_list) < 0) {
        perror("sem_close: не удалось успешно удалить отображение семафора. код: list\n");
    }
    
    // удаляем отображение семафора в виртуальном адресное пространстве программы
    if (sem_close(sem_list_clients) < 0) {
        perror("sem_close: не удалось успешно удалить отображение семафора. код: list\n");
    }
    
    // удаляем отображение сегмента разделяемой памяти из текущего процесса
    if ( munmap(shm_address, shm_stat.st_size) < 0) {
        perror("munmap: не удалось успешно удалить отображение сегмента разделяемой памяти\n");
    }
    
    delwin(right_wnd);
    
    return NULL;
}

void *shm_history() {
    // ---создание левого и правого окон---
    // находим параметры экрана
    int max_height, max_width;
    getmaxyx(stdscr, max_height, max_width);
    
    struct position_wnd pos_left = {.width = max_height - 5, .height = max_width/2 + max_width/3 - 1, .x = 0, .y = 0};
    
    // создаём левое окно (история сообщений)
    WINDOW *left_wnd = create_window(pos_left);
    if (left_wnd == NULL) {
        perror("left_wnd");
        exit(EXIT_FAILURE);
    }
    wrefresh(left_wnd);
    
    // объявляем цветовую пару
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    
    // открытие сегмента разделяемой памяти
    int shm_history_fd = shm_open("/shm_history", O_RDWR, 0666); // получаем файловый дескриптор
    if (shm_history_fd < 0) {
        perror("shm_open: не удалось создать сегмент разделяемой памяти. код: history\n");
        exit(EXIT_FAILURE);
    }
    
    // получаем статистику сегмента разделяемой памяти
    struct stat shm_stat;
    if (fstat(shm_history_fd, &shm_stat) < 0) {
        perror("fstat: не удалось получить информацию о сегменте. код: history.\n");
        shm_unlink("/shm_history");
        exit(EXIT_FAILURE);
    }
    
    // отображение страниц в виртуальное адресное пространство программы
    struct history *shm_address = mmap(NULL, shm_stat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_history_fd, 0); // создание динамического массива в сегменте
    if (shm_address == MAP_FAILED) {
        perror("mmap: не удалось отобразить страницы сегмента разделяемой памяти в адресном пространстве программы. код: history\n");
        shm_unlink("/shm_history");
        exit(EXIT_FAILURE);
    }
    
    // открытие семафора, который обеспечивает синхронизацию между клиентом и сервером
    sem_t *sem_history = sem_open("/sem_history", 0);
    if (sem_history == SEM_FAILED) {
        perror("sem_open: не удалось создать семафор sem_history. код: history\n");
        shm_unlink("/shm_history");
        munmap(shm_address, shm_stat.st_size);
        exit(EXIT_FAILURE);
    }
    
    int msg_counter = 0; // счётчик кол-ва сообщений
    int msg_counter_memory = 0; // предыдущее значение счётчика
    
    while (!_exit_) {
        // определяем кол-во сообщений в очереди
        while(shm_address[msg_counter].msg[0] != '\0') {
            msg_counter++;
        }
        
        if (msg_counter > msg_counter_memory) {
            msg_counter_memory = msg_counter; // запоминаем новое значение
            
            int y_left = 0;
            
            // выводим сообщения всех участников
            for (int i = 0; shm_address[i].nickname[0] != '\0'; i++) {
                wmove(left_wnd, ++y_left, 1);
                wprintw(left_wnd, "%s: %s", shm_address[i].nickname, shm_address[i].msg);
                wattron(left_wnd, COLOR_PAIR(2));
                wrefresh(left_wnd);
            }
            
            if (msg_counter == shm_stat.st_size/2) {
                
                // получаем статистику сегмента разделяемой памяти
                struct stat shm_stat;
                if (fstat(shm_history_fd, &shm_stat) < 0) {
                    perror("fstat: не удалось получить информацию о сегменте. код: history.\n");
                    shm_unlink("/shm_history");
                    exit(EXIT_FAILURE);
                }
                
                // отображение страниц в виртуальное адресное пространство программы
                struct list *shm_address = mmap(NULL, shm_stat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_history_fd, 0); // создание динамического массива в сегменте
                if (shm_address == MAP_FAILED) {
                    perror("mmap: не удалось отобразить страницы в виртуальное адресное пространство программы. код: history\n");
                    shm_unlink("/shm_history");
                    exit(EXIT_FAILURE);
                }
            }
        }
        
        msg_counter = 0;
    }
    
    /*очистка*/
    // удаляем отображение семафора в виртуальном адресное пространстве программы
    if (sem_close(sem_history) < 0) {
        perror("sem_close: не удалось успешно удалить отображение семафора. код: list\n");
    }
    
    // удаляем отображение сегмента разделяемой памяти из текущего процесса
    if ( munmap(shm_address, shm_stat.st_size) < 0) {
        perror("munmap: не удалось успешно удалить отображение сегмента разделяемой памяти\n");
    }
    
    delwin(left_wnd);
    
    return NULL;
}

void *shm_enter(void *args) {
    // ---создание нижнего окна---
    // находим размеры экрана
    int max_height, max_width;
    getmaxyx(stdscr, max_height, max_width);
    
    // задаём параметры окна
    struct position_wnd pos_down = {.width = 4, .height = max_width, .x = max_height - 4, .y = 0};
    
    // создаём цветовую пару
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    
    // создаём нижнее окно
    WINDOW *down_wnd = create_window(pos_down);
    if (down_wnd == NULL) {
        perror("ошибка при создании нижнего окна\n");
        exit(EXIT_FAILURE);
    }
    wrefresh(down_wnd);
    
    struct list client = *(struct list *)args; // получаем никнейм текущего пользователя
    
    // инициализируем структуру user
    struct history user = {
        .nickname = {0},
        .msg = {0}
    };
    
    // копируем никнейм пользователя в структуру msg
    for (int i = 0; client.nickname[i] != '\0'; i++) {
        user.nickname[i] = client.nickname[i];
    }
    
    // ---открытие сегмента разделяемой памяти---
    int shm_history_fd = shm_open("/shm_history", O_RDWR, 0666); // получаем файловый дескриптор
    if (shm_history_fd < 0) {
        perror("shm_open: не удалось создать сегмент разделяемой памяти. код: history\n");
        exit(EXIT_FAILURE);
    }
    
    // получаем статистику сегмента разделяемой памяти
    struct stat shm_stat;
    if (fstat(shm_history_fd, &shm_stat) < 0) {
        perror("fstat: не удалось получить информацию о сегменте. код: history.\n");
        shm_unlink("/shm_history");
        exit(EXIT_FAILURE);
    }
    
    // отображение страниц в виртуальное адресное пространство программы
    struct history *shm_address = mmap(NULL, shm_stat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_history_fd, 0); // создание динамического массива в сегменте
    if (shm_address == MAP_FAILED) {
        perror("mmap: не удалось отобразить страницы сегмента разделяемой памяти в адресном пространстве программы. код: history\n");
        shm_unlink("/shm_history");
        exit(EXIT_FAILURE);
    }
    
    // ---открытие семафора, который обеспечивает синхронизацию между клиентами---
    sem_t *sem_history_clients = sem_open("/sem_history_clients", 0);
    if (sem_history_clients == SEM_FAILED) {
        perror("sem_open: не удалось создать семафор sem_history_clients. код: history\n");
        shm_unlink("/shm_history");
        munmap(shm_address, shm_stat.st_size);
        exit(EXIT_FAILURE);
    }
    
    int msg_counter = 0; // счётчик сообщений
    int length = 0; // счётчик символов
    
    // ---запись и ввод---
    while (!_exit_) {
        box(down_wnd, 0, 0); // восстанавливаем рамку
        
        // перемещаем курсор в позицию 1:1 данного окна
        wmove(down_wnd, 1, 1);
        wprintw(down_wnd, "Enter: ");
        wattron(down_wnd, COLOR_PAIR(2));
        wrefresh(down_wnd);
        
        // ожидаем ввод сообщения
        echo();
        wgetnstr(down_wnd, user.msg, MAX_MSG);
        noecho();
        wrefresh(down_wnd);
        
        // затираем последний символ введённой строки -> '\n'
        while(user.msg[length] != '\0') {
            length++;
        }
        user.msg[length] = '\0';
        
        // если была введена команда, то вызываем проверку
        if (user.msg[0] == '/') {
            exam_cmd(user.msg, length);
        }
        
        sem_wait(sem_history_clients); // чтобы клиенты друг друга не перезаписывали, устанавливаем синхронизацию
        
        // находим свободное место в сегменте
        for (int i = 0; shm_address[msg_counter].msg[0] != '\0'; i++) {
            msg_counter++;
        }
        
        // записываем никнейм пользователя в сегмент
        for (int i = 0; user.nickname[i] != '\0'; i++) {
            shm_address[msg_counter].nickname[i] = user.nickname[i];
        }
        
        // записываем сообщение, которое он отправил
        for (int i = 0; user.msg[i] != '\0';  i++) {
            shm_address[msg_counter].msg[i] = user.msg[i];
        }
        
        msg_counter = 0; // обнуляем счётчик
        
        length = 0; // обнуляем счётчик
        
        sem_post(sem_history_clients); // выход из критической зоны
        
        wclear(down_wnd); // очищаем окно
    }
    
    /*очистка*/
    // удаляем отображение семафора в виртуальном адресное пространстве программы
    if (sem_close(sem_history_clients) < 0) {
        perror("sem_close: не удалось успешно удалить отображение семафора. код: list\n");
    }
    
    // удаляем отображение сегмента разделяемой памяти из текущего процесса
    if ( munmap(shm_address, shm_stat.st_size) < 0) {
        perror("munmap: не удалось успешно удалить отображение сегмента разделяемой памяти\n");
    }
    
    delwin(down_wnd);
    
    return NULL;
}

void exam_cmd(char *msg, int length) {
    int count = 0; // счётчик для подсчёта совпадений
    
    // ---[проверка на команду длиной 5]---
    if (length == 5) {
        char exit_check[] = "/exit"; // строка для сравнения
        
        for (int i = 0; i < length; i++) {
            if (msg[i] == exit_check[i])
                count++;
        }
        
        if (count == 5)
            _exit_ = true;
        else
            count = 0;
        
        // ...место для проверки на другие команды длиной 5...
    }
    
    // ...место для других проверок...
}

WINDOW *create_window(struct position_wnd param) {
    pthread_mutex_lock(&wnd_mutex);
    WINDOW* wnd = newwin(param.width, param.height, param.x, param.y);
    box(wnd, 0, 0);
    pthread_mutex_unlock(&wnd_mutex);
    wrefresh(wnd);
    return (wnd);
}

