#include "app.h"

void bin_operation(int choice) {
  struct bitmap* (*bmap_bit_op)(struct bitmap*, struct bitmap*) =
      dlsym(plugins[choice].handle, plugins[choice].name);
  if (!bmap_bit_op) {
    printf("Не удалось найти функцию %s", PLUGIN_NAME[choice]);
    return;
  }
  int index1, index2;
  struct bitmap* bm;

  index1 = get_number_input("Введите номер первого операнда: ");
  if (index1 < 0 || index1 >= bitmap_count || !bitmaps[index1]) {
    printf(COLOR_ERROR "Неверный номер bitmap.\n" COLOR_RESET);
    return;
  }
  index2 = get_number_input("Введите номер второго операнда: ");
  if (index2 < 0 || index2 >= bitmap_count || !bitmaps[index2]) {
    printf(COLOR_ERROR "Неверный номер bitmap.\n" COLOR_RESET);
    return;
  }
  bm = bmap_bit_op(bitmaps[index1], bitmaps[index2]);
  if (bm == NULL) {
    printf(COLOR_ERROR "Не удалось выполнить операцию.\n" COLOR_RESET);
    return;
  }
  bitmaps[bitmap_count++] = bm;

  printf("Первый битмап:\n");
  print_bits(bitmaps[index1]);
  printf("Второй битмап:\n");
  print_bits(bitmaps[index2]);

  printf(COLOR_PROMPT "Результат сохранен в <bitmap %d>.\n" COLOR_RESET,
         bitmap_count - 1);
  print_bits(bitmaps[bitmap_count - 1]);
}

void un_operation(int choice) {
  int (*bmap_bit_op)(struct bitmap*) =
      dlsym(plugins[choice].handle, plugins[choice].name);
  if (!bmap_bit_op) {
    printf("Не удалось найти функцию %s", PLUGIN_NAME[choice]);
    return;
  }
  int index1;

  index1 = get_number_input("Введите номер операнда: ");
  if (index1 < 0 || index1 >= bitmap_count || !bitmaps[index1]) {
    printf(COLOR_ERROR "Неверный номер bitmap.\n" COLOR_RESET);
    return;
  }
  print_bits(bitmaps[index1]);
  int code = bmap_bit_op(bitmaps[index1]);
  if (code) {
    printf(COLOR_ERROR "Не удалось выполнить операцию.\n" COLOR_RESET);
    return;
  }
  printf(COLOR_PROMPT "Результат перезаписан в <bitmap %d>.\n" COLOR_RESET,
         index1);
  print_bits(bitmaps[index1]);
}
