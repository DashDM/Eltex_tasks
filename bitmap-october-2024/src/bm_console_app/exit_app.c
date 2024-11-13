#include "app.h"

void exit_app() {
  printf("Выход...\n");
  for (int i = 0; i < bitmap_count; i++) {
    bmap_delete(&bitmaps[i]);
  }
}