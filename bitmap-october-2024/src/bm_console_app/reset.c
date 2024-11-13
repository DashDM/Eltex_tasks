#include "app.h"

void reset_bitmap() {
  int index = get_number_input("Введите номер bitmap: ");
  if (index >= 0 && index < bitmap_count && bitmaps[index]) {
    bmap_reset(bitmaps[index]);

    printf(COLOR_PROMPT "Bitmap успешно сброшен.\n" COLOR_RESET);
    print_bits(bitmaps[index]);
  } else {
    printf(COLOR_ERROR "Неверный номер bitmap.\n" COLOR_RESET);
  }
}
