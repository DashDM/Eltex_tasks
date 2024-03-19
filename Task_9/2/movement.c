#include "manager_functional.h"

//блок функционала
int movement (struct catalog **c_wnd, WINDOW *wnd, int f_size, int choice) {
    int ch = 0; //ввод символа с клавиатуры (int для ASCII)
    int exam = 0; //проверка желаем ли мы выйти или переключиться
    int x = 1, y = 1; //координаты окна
    //выбранное курсором отображаем с помощью подсветки
    wattron(wnd, A_REVERSE);
    wmove(wnd, y, x);
    wattroff(wnd, A_REVERSE);
    
    keypad(wnd, TRUE);
    wrefresh(wnd);
    
    bool cycle = true;
    while (cycle) {
        ch = wgetch(wnd);
        
        switch (ch) {
            case KEY_UP:
                if (y != 1) {
                    y--;
                } else {
                    y = f_size;
                }
                break;
            case KEY_DOWN:
                if (y != f_size) {
                    y++;
                } else {
                    y = 1;
                }
                break;
            case 9:
                if (choice == 1) {
                    exam = 2;
                    cycle = false;
                } else {
                    exam = 1;
                    cycle = false;
                }
                break;
            case 27:
                exam = 3;
                cycle = false;
                break;
        }
        print_catalog(*c_wnd, f_size, wnd, y);
    }
    
    return exam;
}
