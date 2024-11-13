#include "app.h"

void greater_than() {
  int f_index = get_number_input("Введите номер первого битмапа: ");
  int s_index = get_number_input("Введите номер второго битмапа: ");

  if (f_index < 0 || f_index >= bitmap_count || bitmaps[f_index] == NULL) {
    printf(COLOR_ERROR "Неверный номер первого битмапа!\n" COLOR_RESET);
    return;
  } else if (s_index < 0 || s_index >= bitmap_count ||
             bitmaps[s_index] == NULL) {
    printf(COLOR_ERROR "Неверный номер второго битмапа!\n" COLOR_RESET);
    return;
  }

  printf("Первый битмап:\n");
  print_bits(bitmaps[f_index]);
  printf("Второй битмап:\n");
  print_bits(bitmaps[s_index]);

  printf("Первый битмап > Второй битмап -> ");
  if (bmap_is_greater(bitmaps[f_index], bitmaps[s_index])) {
    printf(COLOR_PROMPT "TRUE\n" COLOR_RESET);
  } else {
    printf(COLOR_ERROR "FALSE\n" COLOR_RESET);
  }
}

void less_than() {
  int f_index = get_number_input("Введите номер первого битмапа: ");
  int s_index = get_number_input("Введите номер второго битмапа: ");

  if (f_index < 0 || f_index >= bitmap_count || bitmaps[f_index] == NULL) {
    printf(COLOR_ERROR "Неверный номер первого битмапа!\n" COLOR_RESET);
    return;
  } else if (s_index < 0 || s_index >= bitmap_count ||
             bitmaps[s_index] == NULL) {
    printf(COLOR_ERROR "Неверный номер второго битмапа!\n" COLOR_RESET);
    return;
  }

  printf("Первый битмап:\n");
  print_bits(bitmaps[f_index]);
  printf("Второй битмап:\n");
  print_bits(bitmaps[s_index]);

  printf("Первый битмап < Второй битмап -> ");
  if (bmap_is_less(bitmaps[f_index], bitmaps[s_index])) {
    printf(COLOR_PROMPT "TRUE\n" COLOR_RESET);
  } else {
    printf(COLOR_ERROR "FALSE\n" COLOR_RESET);
  }
}

void equal() {
  int f_index = get_number_input("Введите номер первого битмапа: ");
  int s_index = get_number_input("Введите номер второго битмапа: ");

  if (f_index < 0 || f_index >= bitmap_count || bitmaps[f_index] == NULL) {
    printf(COLOR_ERROR "Неверный номер первого битмапа!\n" COLOR_RESET);
    return;
  } else if (s_index < 0 || s_index >= bitmap_count ||
             bitmaps[s_index] == NULL) {
    printf(COLOR_ERROR "Неверный номер второго битмапа!\n" COLOR_RESET);
    return;
  }

  printf("Первый битмап:\n");
  print_bits(bitmaps[f_index]);
  printf("Второй битмап:\n");
  print_bits(bitmaps[s_index]);
  
  printf("Первый битмап = Второй битмап -> ");
  if (bmap_is_eq(bitmaps[f_index], bitmaps[s_index])) {
    printf(COLOR_PROMPT "TRUE\n" COLOR_RESET);
  } else {
    printf(COLOR_ERROR "FALSE\n" COLOR_RESET);
  }
}

void greater_or_equal() {
  int f_index = get_number_input("Введите номер первого битмапа: ");
  int s_index = get_number_input("Введите номер второго битмапа: ");

  if (f_index < 0 || f_index >= bitmap_count || bitmaps[f_index] == NULL) {
    printf(COLOR_ERROR "Неверный номер первого битмапа!\n" COLOR_RESET);
    return;
  } else if (s_index < 0 || s_index >= bitmap_count ||
             bitmaps[s_index] == NULL) {
    printf(COLOR_ERROR "Неверный номер второго битмапа!\n" COLOR_RESET);
    return;
  }

  printf("Первый битмап:\n");
  print_bits(bitmaps[f_index]);
  printf("Второй битмап:\n");
  print_bits(bitmaps[s_index]);

  printf("Первый битмап >= Второй битмап -> ");
  if (bmap_is_greater_or_eq(bitmaps[f_index], bitmaps[s_index])) {
    printf(COLOR_PROMPT "TRUE\n" COLOR_RESET);
  } else {
    printf(COLOR_ERROR "FALSE\n" COLOR_RESET);
  }
}

void less_or_equal() {
  int f_index = get_number_input("Введите номер первого битмапа: ");
  int s_index = get_number_input("Введите номер второго битмапа: ");

  if (f_index < 0 || f_index >= bitmap_count || bitmaps[f_index] == NULL) {
    printf(COLOR_ERROR "Неверный номер первого битмапа!\n" COLOR_RESET);
    return;
  } else if (s_index < 0 || s_index >= bitmap_count ||
             bitmaps[s_index] == NULL) {
    printf(COLOR_ERROR "Неверный номер второго битмапа!\n" COLOR_RESET);
    return;
  }

  printf("Первый битмап:\n");
  print_bits(bitmaps[f_index]);
  printf("Второй битмап:\n");
  print_bits(bitmaps[s_index]);
  
  printf("Первый битмап <= Второй битмап -> ");
  if (bmap_is_less_or_eq(bitmaps[f_index], bitmaps[s_index])) {
    printf(COLOR_PROMPT "TRUE\n" COLOR_RESET);
  } else {
    printf(COLOR_ERROR "FALSE\n" COLOR_RESET);
  }
}
