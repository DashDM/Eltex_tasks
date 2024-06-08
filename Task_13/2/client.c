#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <ncurses.h>
#include <pthread.h>

#define MAX_MSG 256
#define MAX_NICKNAME 10
#define MAX_HISTORY 256
#define SERVER_TYPE 1

volatile bool _exit_ = false; // чтобы завершить потоки

pthread_mutex_t wnd_mutex = PTHREAD_MUTEX_INITIALIZER; // mutex, чтобы окна создавались поочерёдно

// структура | используется как кадр для ЗАПИСИ одного сообщения в очередь
struct user_data {
    long type; // приоритет для чтения из очереди
    char nickname[MAX_NICKNAME]; // ник пользователя
    char msg[MAX_MSG]; // сообщение, что он отправил
    int uid; // идентификатор пользователя | используется в качестве приоритета для передачи "server -> client"
    short status; // статус пользователя (0 - оффлайн, 1 - онлайн, 2 - отправляет сообщение) | используется для выхода и цветовой пары в списке участников
};

struct chat_data {
    long type; // приоритет для записи/чтения
    char nickname[MAX_NICKNAME]; // история сообщений (кто написал) | ник пользователя из списка участников чата
    char msg[MAX_MSG]; // история сообщений (что написал)
    int list_size; // сколько всего участников | используется для сброса координаты Y при выводе на экран
    int history_size; // сколько всего сообщений | используется для сброса координаты Y при выводе на экран
    short status; // статус пользователя (0 - оффлайн, 1 - онлайн, 2 - вводит сообщение) | используется для выхода и цветовой пары в списке участников
};

struct client_ids {
    int pid; // process id
    int qid; // queue id
};

// структура с параметрами для создания окна
struct position_wnd {
    int width; // ширина
    int height; // высота
    int x; // координата по оси х
    int y; // координата по оси у
};

void *take_panels(void *args); // вывод истории сообщений и списка участников на экран

void *send_panel(void *args); // отправка сообщений в чат

void exam_cmd(char *msg, int length); // проверка на введённую команду

void del_newline(struct user_data *user); // удаления символа '\n'

WINDOW *create_window(struct position_wnd param); // создание окна

int main(void) {
    // ---открытие очереди сообщений---
    int key = ftok("client.c", 5); // генерируем ключ
    
    struct client_ids client = {
        .pid = getpid(), // получаем идентификатор процесса и сохраняем его в client.pid
        .qid = msgget(key, 0) // открываем очередь и сохраняем её идентификатор в client.qid
    };
    
    if (client.qid < 0) {
        perror("ошибка при открытии очереди\n");
        exit(EXIT_FAILURE);
    }
    
    // ---инициализвация структуры текущего пользователя---
    struct user_data user = {
        .type = SERVER_TYPE,
        .nickname = {0},
        .msg = {0},
        .uid = client.pid,
        .status = 1
    };
    
    // ---регистрация нового пользователя---
    printf("Enter ur nickname: ");
    fgets(user.nickname, MAX_NICKNAME, stdin);
    
    if (msgsnd(client.qid, (void*)&user, sizeof(struct user_data), 0) < 0) {
        perror("user.nickname -> msgsnd\n");
        exit(EXIT_FAILURE);
    }
    
    user.type = client.qid;
    
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
    
    // ---создание потоков---
    pthread_t threads[2]; // 0 - поток для записи сообщений в очередь, 1 - поток для чтения сообщений из очереди
    
    pthread_create(&threads[0], NULL, take_panels, (void *)&client);
    pthread_create(&threads[1], NULL, send_panel, (void *)&user);
    
    // --ожидание закрытия потоков--
    for (int i = 0; i < 2; i++) {
        pthread_join(threads[i], NULL);
    }
    
    endwin();
    
    return 0;
}


void *take_panels(void *args) {
    struct client_ids client = *(struct client_ids *)args; // содержит идентификатор процесса и очереди сообщений  (.pid и .qid сответственно)
    
    // ---инициализируем структуру для чтения сообщений из очереди---
    struct chat_data take = {
        .type = 0,
        .nickname = {0},
        .msg = {0},
        .list_size = 0,
        .history_size = 0,
        .status = 0
    };
    
    // ---создание левого и правого окон---
    // находим параметры экрана
    int max_height, max_width;
    getmaxyx(stdscr, max_height, max_width);
    
    // задаём параметры окна
    struct position_wnd pos_right = {.width = max_height - 5, .height = max_width/2 - max_width/3 - 1, .x = 0, .y = max_width/2 + max_width/3 + 1};
    struct position_wnd pos_left = {.width = max_height - 5, .height = max_width/2 + max_width/3 - 1, .x = 0, .y = 0};
    
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
    
    // создаём левое окно (история сообщений)
    WINDOW *left_wnd = create_window(pos_left);
    if (left_wnd == NULL) {
        perror("left_wnd");
        exit(EXIT_FAILURE);
    }
    wrefresh(left_wnd);
    
    int y_left = 0, y_right = 0; // ось Y для каждого окна
    
    // ---чтение и вывод на экран---
    while (!_exit_) {
        if (msgrcv(client.qid, (void *)&take, sizeof(struct chat_data), client.pid, IPC_NOWAIT) < 0) {
            // если _exit_ стал ture, завершаем цикл и завершаем поток
            if (_exit_) {
                break;
            }
        } else {
            if (take.status != 2) {
                // ---в список участников---
                if (take.status == 1) {
                    wmove(right_wnd, ++y_right, 1);
                    wattron(right_wnd, COLOR_PAIR(2));
                    wprintw(right_wnd, "%s", take.nickname);
                    wrefresh(right_wnd);
                } else if (take.status == 0) {
                    wmove(right_wnd, ++y_right, 1);
                    wattron(right_wnd, COLOR_PAIR(1));
                    wprintw(right_wnd, "%s", take.nickname);
                    wrefresh(right_wnd);
                }
                
                if (y_right == take.list_size)
                        y_right = 0;
            } else {
                // ---в историю сообщений---
                wmove(left_wnd, ++y_left, 1);
                wprintw(left_wnd, "%s: %s", take.nickname, take.msg);
                wattron(left_wnd, COLOR_PAIR(2));
                wrefresh(left_wnd);
                
                if (y_left == take.history_size)
                    y_left = 0;
            }
        }
    }
    
    delwin(left_wnd);
    delwin(right_wnd);
    
    return NULL;
}

void *send_panel(void *args) {
    struct user_data user = *(struct user_data *)args;
    int qid = user.type; // извлекаем из структуры id очереди сообщений
    user.type = SERVER_TYPE; // делаем приоритет = SERVER_TYPE, чтобы только сервер его прочитал
    user.status = 2; // статус пользователя переводим в 2 (вводит сообщение)
    
    int length = 0; // длина введённого сообщения
    
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
        
        del_newline(&user);
        
        // находим длину введённого сообщения
        while(user.msg[length] != '\0') {
            length++;
        }
        
        // если была введена команда, то вызываем проверку
        if (user.msg[0] == '/') {
            exam_cmd(user.msg, length);
            
            if (_exit_ == true) {
                user.status = 0;
            }
        }
        
        // отправляем сообщение в очередь
        if (msgsnd(qid, (void*)&user, sizeof(struct user_data), 0) < 0) {
            perror("user -> msgsnd\n");
            exit(EXIT_FAILURE);
        }
        
        // зануляем поле msg
        for (int i = 0; i < length; i++) {
            user.msg[i] = '\0';
        }
        
        length = 0; // обнуляем счетчик длины сообщения
        
        sleep(1); // халтура, чтобы не возиться с синхронизацией
        wclear(down_wnd);
    }
    
    delwin(down_wnd);
    
    return NULL;
}

// функция для создания окна
WINDOW *create_window(struct position_wnd param) {
    pthread_mutex_lock(&wnd_mutex);
    WINDOW* wnd = newwin(param.width, param.height, param.x, param.y);
    box(wnd, 0, 0);
    pthread_mutex_unlock(&wnd_mutex);
    wrefresh(wnd);
    return (wnd);
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

void del_newline(struct user_data *user) {
    int length = 0;
    if (user->nickname[length] != '\0') {
        while (user->nickname[length] != '\0') {
            if (user->nickname[length] == '\n') {
                user->nickname[length] = '\0';
                break;
            }
            length++;
        }
    }
    
    length = 0;
    
    if (user->msg[length] == '\n') {
        user->msg[0] = '\0';
    } else {
        if (user->msg[length] != '\0')
        while (user->msg[length] != '\0') {
            if (user->msg[length] == '\n') {
                user->msg[length] = '\0';
                break;
            }
            length++;
        }
    }
}
