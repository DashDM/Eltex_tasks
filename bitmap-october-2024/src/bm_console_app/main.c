#include "app.h"

struct bitmap *bitmaps[MAX_BITMAPS] = {NULL};
int bitmap_count = 0;
struct plugin plugins[PLUGIN_COUNT];
int main() {
  setlocale(LC_ALL, "Rus");
  open_plugins();
  run();
  close_plugins();
  return 0;
}

void run() {
  int choice;
  while (1) {
    print_menu();
    choice = get_number_input("Выберите пункт меню: ");
    switch (choice) {
      case 1:
        add_bitmap();
        break;
      case 2:
        delete_bitmap();
        break;
      case 3:
        list_bitmaps();
        break;
      case 4:
        edit_bitmap();
        break;
      case 5:
        bit_operations();
        break;
      case 6:
        comparison_operations();
        break;
      case 7:
        copy_bitmap();
        break;
      case 8:
        clone_bitmap();
        break;
      case 9:
        merge_bitmaps();
        break;
      case 10:
        reset_bitmap();
        break;
      case 11:
        exit_app();
        return;
      default:
        printf(
            COLOR_ERROR
            "Неверный ввод. Пожалуйста, выберите пункт из меню.\n" COLOR_RESET);
        break;
    }
  }
}

void print_menu() {
  printf(COLOR_MENU "\n------- Меню -------\n" COLOR_RESET);
  printf(COLOR_TEXT " 1. Добавить bitmap\n");
  printf(" 2. Удалить bitmap\n");
  printf(" 3. Список bitmap\n");
  printf(" 4. Изменить bitmap\n");
  printf(" 5. Битовые операции\n");
  printf(" 6. Операции сравнения\n");
  printf(" 7. Копировать bitmap\n");
  printf(" 8. Клонировать bitmap\n");
  printf(" 9. Склеить два bitmap\n");
  printf("10. Сбросить bitmap\n");
  printf("11. Выход\n" COLOR_RESET);
}

void bit_operations() {
  printf(COLOR_MENU "\n--- Битовые операции ---\n" COLOR_RESET);
  for (int i = 0; i < PLUGIN_COUNT; ++i) {
    if (plugins[i].connected)
      printf(COLOR_PROMPT);
    else
      printf(COLOR_ERROR);
    printf("%d. Выполнить %s\n" COLOR_RESET, i + 1, plugins[i].name);
  }
  printf(COLOR_TEXT "%d. Назад.\n" COLOR_RESET, PLUGIN_COUNT + 1);
  int choice = get_number_input("Выберите операцию: ");
  switch (choice) {
    case 1:
      if (plugins[choice - 1].connected)
        bin_operation(choice - 1);
      else
        printf(COLOR_ERROR "\nПлагин не подключен.\n" COLOR_RESET);
      break;
    case 2:
      if (plugins[choice - 1].connected)
        bin_operation(choice - 1);
      else
        printf(COLOR_ERROR "\nПлагин не подключен.\n" COLOR_RESET);
      break;
    case 3:
      if (plugins[choice - 1].connected)
        bin_operation(choice - 1);
      else
        printf(COLOR_ERROR "\nПлагин не подключен.\n" COLOR_RESET);
      break;
    case 4:
      if (plugins[choice - 1].connected)
        un_operation(choice - 1);
      else
        printf(COLOR_ERROR "\nПлагин не подключен.\n" COLOR_RESET);
      break;
    case 5:
      break;
    default:
      printf(COLOR_ERROR "Неверный ввод.\n" COLOR_RESET);
      break;
  }
}

void comparison_operations() {
  printf(COLOR_MENU "\n--- Операции сравнения ---\n" COLOR_RESET);
  printf(COLOR_TEXT "1. Операция >\n");
  printf("2. Операция <\n");
  printf("3. Операция =\n");
  printf("4. Операция >=\n");
  printf("5. Операция <=\n");
  printf("6. Назад\n" COLOR_RESET);
  int choice = get_number_input("Выберите операцию: ");
  switch (choice) {
    case 1:
      greater_than();
      break;
    case 2:
      less_than();
      break;
    case 3:
      equal();
      break;
    case 4:
      greater_or_equal();
      break;
    case 5:
      less_or_equal();
      break;
    case 6:
      break;
    default:
      printf(COLOR_ERROR "Неверный ввод.\n" COLOR_RESET);
      break;
  }
}

int get_number_input(const char *prompt) {
  int number;
  printf("%s", prompt);
  while (scanf("%d", &number) != 1) {
    printf(COLOR_ERROR "Ошибка ввода. Введите число: " COLOR_RESET);
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
      ;
  }
  return number;
}

int get_bitmap_input(int *index1, int *index2, const char *prompt1,
                     const char *prompt2) {
  *index1 = get_number_input(prompt1);
  if (index2 != NULL) {
    *index2 = get_number_input(prompt2);
  }
  return (*index1 >= 0 && (index2 == NULL || *index2 >= 0)) ? 1 : 0;
}

void clear_stdin() {
  int ch;
  while ((ch = getchar()) != '\n' && ch != EOF)
    ;
}

void open_plugins() {
  for (int i = 0; i < PLUGIN_COUNT; ++i) {
    plugins[i].handle = NULL;
    strcpy(plugins[i].path, PLUGIN_PATH[i]);
    strcpy(plugins[i].name, PLUGIN_NAME[i]);
    plugins[i].connected = 0;
  }
  while (1) {
    print_plugins();
    int choice = get_number_input("Введите вариант: ");
    if (choice > 0 && choice <= PLUGIN_COUNT) {
      if (plugins[choice - 1].connected) {
        dlclose(plugins[choice - 1].handle);
        plugins[choice - 1].connected = 0;
      } else {
        plugins[choice - 1].handle =
            dlopen(plugins[choice - 1].path, RTLD_LAZY);
        if (!plugins[choice - 1].handle) {
          printf(COLOR_ERROR "Не удалось найти плагин\n" COLOR_RESET);
          continue;
        }
        plugins[choice - 1].connected = 1;
      }
    } else if (choice == PLUGIN_COUNT + 1) {
      break;
    } else
      printf(COLOR_ERROR "Неправильный ввод\n" COLOR_RESET);
  }
}

void print_plugins() {
  printf(COLOR_MENU "\n------- Выбор плагинов -------\n" COLOR_RESET);
  for (int i = 0; i < PLUGIN_COUNT; ++i) {
    if (plugins[i].connected) {
      printf(COLOR_PROMPT);
    }
    printf("%d. %s\n", i + 1, plugins[i].name);
    if (plugins[i].connected) {
      printf(COLOR_RESET);
    }
  }
  printf("\n%d. Готово\n", PLUGIN_COUNT + 1);
}
void close_plugins() {
  for (int i = 0; i < PLUGIN_COUNT; ++i) {
    if (plugins[i].connected) {
      dlclose(plugins[i].handle);
    }
  }
}
