#include "app.h"

void clone_bitmap() {
  int index = get_number_input("Введите номер bitmap: ");
  struct bitmap *bm;
  if (index >= 0 && index < bitmap_count && bitmaps[index]) {
    bm = bmap_clone(bitmaps[index]);

    if (bm == NULL) {
      printf(COLOR_ERROR "Не удалось выполнить клонирование.\n" COLOR_RESET);
      return;
    }

    bitmaps[bitmap_count++] = bm;
    printf(
        COLOR_PROMPT
        "Bitmap успешно склонирован и сохранен под номером %d.\n" COLOR_RESET,
        bitmap_count - 1);
    print_bits(bitmaps[bitmap_count - 1]);
  } else {
    printf(COLOR_ERROR "Неверный номер bitmap.\n" COLOR_RESET);
  }
}
