#include "app.h"

void copy_bitmap() {
  int index_src, index_dest, start, count, pos;

  index_src = get_number_input("Введите номер bitmap источника: ");
  if (index_src < 0 || index_src >= bitmap_count ||
      bitmaps[index_src] == NULL) {
    printf(COLOR_ERROR "Неверный номер bitmap!\n" COLOR_RESET);
    return;
  }

  start = get_number_input("Введите индекс начала копирования: ");
  if (start < 0 || start >= (int)bitmaps[index_src]->size) {
    printf(COLOR_ERROR "Неверный индекс начала!\n" COLOR_RESET);
    return;
  }

  count = get_number_input(
      "Введите количество битов, которые нужно скопировать, начиная с этого "
      "индекса: ");
  if (count <= 0 || (int)bitmaps[index_src]->size < start + count) {
    printf(COLOR_ERROR
           "Количество битов выходит за границы bitmap!\n" COLOR_RESET);
    return;
  }

  index_dest = get_number_input("Введите номер bitmap назначения: ");
  if (index_dest < 0 || index_dest >= bitmap_count ||
      bitmaps[index_dest] == NULL) {
    printf(COLOR_ERROR "Неверный номер bitmap!\n" COLOR_RESET);
    return;
  }

  pos = get_number_input("Введите индекс места вставки: ");
  if (pos < 0 || pos > (int)bitmaps[index_dest]->size) {
    printf(COLOR_ERROR "Неверный индекс начала!\n" COLOR_RESET);
    return;
  }

  if (bmap_copy(bitmaps[index_dest], bitmaps[index_src], pos, start, count) ==
      -1) {
    printf(COLOR_ERROR "Не удалось выполнить копирование.\n" COLOR_RESET);
  } else {
    printf(COLOR_PROMPT "Копирование выполнено успешно.\n" COLOR_RESET);
    print_bits(bitmaps[index_dest]);
  }
}
