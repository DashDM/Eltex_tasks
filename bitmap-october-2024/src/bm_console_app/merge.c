#include "app.h"

void merge_bitmaps() {
  int index1, index2;
  struct bitmap *bm;

  index1 = get_number_input("Введите номер первого bitmap: ");
  if (index1 < 0 || index1 >= bitmap_count || !bitmaps[index1]) {
    printf(COLOR_ERROR "Неверный номер bitmap.\n" COLOR_RESET);
    return;
  }

  index2 = get_number_input("Введите номер второго bitmap: ");
  if (index2 < 0 || index2 >= bitmap_count || !bitmaps[index2]) {
    printf(COLOR_ERROR "Неверный номер bitmap.\n" COLOR_RESET);
    return;
  }

  bm = bmap_cat(bitmaps[index1], bitmaps[index2]);
  if (bm == NULL) {
    printf(COLOR_ERROR "Не удалось выполнить склеивание.\n" COLOR_RESET);
    return;
  }

  bitmaps[bitmap_count++] = bm;
  printf(COLOR_PROMPT "Результат сохранен в <bitmap %d>.\n" COLOR_RESET,
         bitmap_count - 1);
  print_bits(bitmaps[bitmap_count - 1]);
}
