#include "manager_functional.h"
// блок создания окон
WINDOW *create_left_window(int max_height, int max_width) {
    WINDOW* left_wnd = newwin(max_height - 5, max_width/2 - 1, 0, 0);
    box(left_wnd, '|', '-');
    wrefresh(left_wnd);
    return (left_wnd);
}

WINDOW *create_right_window(int max_height, int max_width) {
    WINDOW* right_wnd = newwin(max_height - 5, max_width/2 - 1, 0, max_width/2 + 1);
    box(right_wnd, '|', '-');
    wrefresh(right_wnd);
    return (right_wnd);
}

WINDOW *create_down_window(int max_height, int max_width) {
    WINDOW* down_wnd = newwin(4, max_width, max_height - 4, 0);
    box(down_wnd, '|', '-');
    wrefresh(down_wnd);
    return (down_wnd);
}
