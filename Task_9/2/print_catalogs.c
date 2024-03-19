#include "manager_functional.h"

void print_catalog(struct catalog *c_wnd, int f_size, WINDOW *wnd, int y) {
    int max_height, max_width;
    getmaxyx(wnd, max_height, max_width);
    
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    
    int j = 0;
    for(int i = 0; i < f_size; i++) {
        if (y == i + 1) {
            wmove(wnd, ++j, 1);
            wattron(wnd, A_REVERSE);
            wprintw(wnd, "%s", c_wnd[i].file_name);
            wmove(wnd, j, max_width - 8);
            wprintw(wnd, "%ld", c_wnd[i].file_size);
            wattroff(wnd, A_REVERSE);
        } else {
            wmove(wnd, ++j, 1);
            wprintw(wnd, "%s", c_wnd[i].file_name);
            wmove(wnd, j, max_width - 8);
            wprintw(wnd, "%ld", c_wnd[i].file_size);
        }
        wattron(wnd, COLOR_PAIR(2));
        wbkgd(wnd, COLOR_PAIR(1));
    }
    wrefresh(wnd);
}

void print_down_catalog(WINDOW *wnd) {
    int max_height, max_width;
    getmaxyx(wnd, max_height, max_width);
    
    wmove(wnd, 1, 1);
    wprintw(wnd, "Instruction:\n U can use a keyboard to move! But only u can it's ARROW-UP and ARROW-DOWN. And also u can use the TAB to switch and ESC to leave!");
}

