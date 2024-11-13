#include "app.h"

void delete_bitmap() {
  int index = get_number_input("Введите номер bitmap: ");
  if (index >= 0 && index < bitmap_count && bitmaps[index] != NULL) {
    bmap_delete(&bitmaps[index]);
    bitmap_count--;

    for (int i = index; i < bitmap_count; i++) {
      bitmaps[i] = bitmaps[i + 1];
    }
    printf(COLOR_PROMPT "Bitmap успешно удален.\n" COLOR_RESET);
  } else {
    printf(COLOR_ERROR "Неверный номер bitmap.\n" COLOR_RESET);
  }
}
