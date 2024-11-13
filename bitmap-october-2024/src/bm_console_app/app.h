#ifndef APP_H
#define APP_H

#include <dlfcn.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/bitmap.h"

#define PLUGIN_PATH                                           \
  (const char * [4]) {                                        \
    "bmap_and.so", "bmap_or.so", "bmap_xor.so", "bmap_not.so" \
  }
#define PLUGIN_NAME \
  (const char * [4]) { "bmap_and", "bmap_or", "bmap_xor", "bmap_not" }
#define PLUGIN_COUNT 4
#define PLUGIN_MAX_LEN 30

#define MAX_BITMAPS 1000
#define MAX_BITMAP_SIZE 10000
#define BITS_PER_GROUP 8
#define BYTES_PER_LINE 6

#define COLOR_BLUE "\033[38;5;12m"
#define COLOR_GREY "\033[38;5;242m"
#define COLOR_RESET "\033[0m"
#define COLOR_MENU "\033[95m"
#define COLOR_TEXT "\033[33m"
#define COLOR_PROMPT "\033[1;32m"
#define COLOR_ERROR "\033[1;31m"

extern struct bitmap *bitmaps[MAX_BITMAPS];
extern int bitmap_count;

struct plugin {
  void *handle;
  char path[PLUGIN_MAX_LEN];
  char name[PLUGIN_MAX_LEN];
  int connected;
};
extern struct plugin plugins[PLUGIN_COUNT];

void open_plugins();
void print_plugins();
void close_plugins();

void print_menu();
void clear_stdin();
void print_bits(struct bitmap *bm);
void print_byte_number(int num);

void add_bitmap();
void delete_bitmap();
void list_bitmaps();

void edit_bitmap();

void bit_operations();
void bin_operation(int choice);
void un_operation(int choice);

void comparison_operations();
void greater_than();
void less_than();
void equal();
void greater_or_equal();
void less_or_equal();

void copy_bitmap();
void clone_bitmap();
void merge_bitmaps();
void reset_bitmap();

int get_bitmap_input(int *index1, int *index2, const char *prompt1,
                     const char *prompt2);
int get_number_input(const char *prompt);

void run();
void exit_app();

#endif
