#include "app.h"

void list_bitmaps() {
  printf(COLOR_PROMPT "\n--- Список bitmap ---\n" COLOR_RESET);
  for (int i = 0; i < bitmap_count; i++) {
    if (bitmaps[i]) {
      printf(COLOR_BLUE "[%d] <bitmap %d> size=%ld\n" COLOR_RESET, i, i,
             bitmaps[i]->size);

      print_bits(bitmaps[i]);
      printf("\n");
    }
  }
}

void print_bits(struct bitmap *bm) {
  if (bm == NULL) return;

  int bit, n = 0;
  for (size_type i = 0; i < bm->size; i++) {
    if (i != 0 && i % BITS_PER_GROUP == 0) {
      printf(" ");
    }

    if (i != 0 && i % (BYTES_PER_LINE * BITS_PER_GROUP) == 0) {
      printf("\n");
      for (int j = 0; j < BYTES_PER_LINE; j++) {
        n = i / BITS_PER_GROUP + j - BYTES_PER_LINE + 1;
        print_byte_number(n - 1);
        printf(" ");
      }
      printf("\n");
    }

    bmap_getbit(bm, i, &bit);
    printf("%d", bit);
  }
  printf("\n");
  for (int i = 0; i < BYTES_PER_LINE; i++) {
    print_byte_number(n + i);
    printf(" ");
  }
  printf("\n");
}

void print_byte_number(int num) {
  char buffer[BITS_PER_GROUP];

  int len = snprintf(buffer, sizeof(buffer), "[%d]", num);

  int padding = (BITS_PER_GROUP - len) / 2;
  int padding_right = BITS_PER_GROUP - len - padding;

  for (int i = 0; i < padding; i++) {
    printf(" ");
  }
  printf(COLOR_GREY "%s" COLOR_RESET, buffer);
  for (int i = 0; i < padding_right; i++) {
    printf(" ");
  }
}
