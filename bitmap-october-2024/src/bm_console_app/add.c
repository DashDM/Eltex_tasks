#include "app.h"

void add_bitmap() {
  int size;
  size_type counter = 0;
  char c;
  struct bitmap *bm;

  printf("Введите размер: ");
  scanf("%d", &size);

  if (size <= 0 || size > MAX_BITMAP_SIZE) {
    printf(COLOR_ERROR
           "Веденное число должно быть положительным и не превышать "
           "%d!\n" COLOR_RESET,
           MAX_BITMAP_SIZE);
    return;
  }

  printf(
      "Введите последовательность битов (невведенные биты заполнятся "
      "нулями):\n");

  if (bmap_init(&bm, size) == -1) {
    printf(COLOR_ERROR "Не удалось создать bitmap.\n" COLOR_RESET);
    return;
  }

  bm->size = size;

  clear_stdin();
  while ((c = getchar()) != '\n') {
    if (c != '0' && c != '1') {
      printf(COLOR_ERROR "Встречен недопустимый символ \"%c\"!\n" COLOR_RESET,
             c);
      clear_stdin();
      bmap_delete(&bm);
      return;
    }

    if (counter >= bm->size) {
      printf(
          COLOR_ERROR
          "Количество введенных битов превышает размер bitmap!\n" COLOR_RESET);
      clear_stdin();
      bmap_delete(&bm);
      return;
    }

    bmap_setbit(bm, counter, c - '0');

    counter++;
  }

  bitmaps[bitmap_count++] = bm;
}
