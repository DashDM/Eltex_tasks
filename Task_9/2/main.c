#include "manager_functional.h"

int main(void) {
    //инициализируем динамически массивы структур для информации из директорий
    struct catalog *c_left = malloc(sizeof(struct catalog));
    struct catalog *c_right = malloc(sizeof(struct catalog));
    
    //переменные для того, чтобы знать сколько файлов хранится в директории
    int c_l_size = 0, c_r_size = 0;
    
    //сами директории (для левого и правого окна соответственно)
    char l_directory[name_size] = "/home/dmitry/Desktop/eltex_homework";
    char r_directory[name_size] = "/home/dmitry/Desktop/eltex_homework/Task_9/2";
    
    //читаем каталоги и фиксируем информацию о файлах в структурах
    fill_catalog(&c_left, &c_l_size, l_directory);
    fill_catalog(&c_right, &c_r_size, r_directory);
    
    // инициализируем структуру данных библиотеки ncurses.h и прочие базовые штучки
    initscr();
    clear();
    noecho();
    curs_set(0);
    cbreak();
    
    // инициализируем структуру и определяем цветовую пару для фона
    start_color();
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    bkgd(COLOR_PAIR(1));
    refresh();
    
    //инициализируем окна (левое, правое, нижнее)
    WINDOW *left_wnd;
    WINDOW *right_wnd;
    WINDOW *down_wnd;
    
    //находим параметры экрана
    int max_height, max_width;
    getmaxyx(stdscr, max_height, max_width);
    
    //создаём окна
    left_wnd = create_left_window(max_height, max_width);
    right_wnd = create_right_window(max_height, max_width);
    down_wnd = create_down_window(max_height, max_width);
    
    //для курсора и цикла
    int choice = 1;
    bool cycle = true;
    
    //выводим начальное изображение
    print_catalog(c_left, c_l_size, left_wnd, 1);
    print_catalog(c_right, c_r_size, right_wnd, 1);
    print_down_catalog(down_wnd);
    wrefresh(left_wnd);
    wrefresh(right_wnd);
    wrefresh(down_wnd);
    
    //1, 2, 3 получаются взависимости от нажатия Tab (1,2) или Esc (3)
    while (cycle) {
        switch (choice) {
            case 1:
                choice = movement(&c_left, left_wnd, c_l_size, choice);
                break;
            case 2:
                choice = movement(&c_right, right_wnd, c_r_size, choice);
                break;
            case 3:
                cycle = false;
                break;
        }
    }
    
    //удаляем удаляем
    delwin(left_wnd);
    delwin(right_wnd);
    delwin(down_wnd);
    
    //чистим чистим
    free(c_left);
    free(c_right);
    
    //закрываем
    endwin();
    return 0;
}


