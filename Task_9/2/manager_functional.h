#ifndef __MANAGER_FUNCTIONAL_H__
#define __MANAGER_FUNCTIONAL_H__
#include <stdlib.h>
#include <ncurses.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

#define name_size 255

struct catalog {
    char file_name[name_size];
    long int file_size;
};

WINDOW *create_left_window(int, int);
WINDOW *create_right_window(int, int);
WINDOW *create_down_window(int, int);

void fill_catalog(struct catalog **, int *, char *);

int movement (struct catalog **, WINDOW *, int, int);

void print_catalog(struct catalog *, int, WINDOW *, int);
void print_down_catalog(WINDOW *);
#endif
