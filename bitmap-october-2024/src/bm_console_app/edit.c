#include "app.h"

void edit_bitmap() {
  int index = get_number_input("Введите номер bitmap: ");
  if (index >= 0 && index < bitmap_count && bitmaps[index]) {
    int bit_index = get_number_input("Введите индекс для замены: ");
    if (bit_index < 0 || (size_type)bit_index >= bitmaps[index]->size) {
      printf(COLOR_ERROR "Индекс выходит за границы bitmap!\n" COLOR_RESET);
      return;
    }

    int new_value = get_number_input("Новое значение бита: ");
    if (new_value != 0 && new_value != 1) {
      printf(COLOR_ERROR "Недопустимое значение бита!\n" COLOR_RESET);
      return;
    }

    bmap_setbit(bitmaps[index], (size_type)bit_index, new_value);

    printf(COLOR_PROMPT "Bitmap успешно изменен.\n" COLOR_RESET);
    print_bits(bitmaps[index]);
  } else {
    printf(COLOR_ERROR "Неверный номер bitmap.\n" COLOR_RESET);
  }
}
