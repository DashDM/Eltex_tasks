#include "bmap_test.h"

START_TEST(bmap_getbit_1) {
  int bit = 0;
  struct bitmap *bm;

  bmap_init(&bm, 15);
  bm->bits[0] = 0b01011101;
  bm->bits[1] = 0b00000110;

  bmap_getbit(bm, 14, &bit);
  ck_assert_int_eq(bit, 1);

  bm->bits[0] = 0;
  bm->bits[1] = 8;

  bmap_getbit(bm, 12, &bit);
  ck_assert_int_eq(bit, 1);

  bmap_delete(&bm);
}
END_TEST

START_TEST(bmap_getbit_2) {
  int bit = 0;
  struct bitmap *bm = NULL;

  int code = bmap_getbit(bm, 14, &bit);
  ck_assert_int_eq(code, FAILURE_CODE);
}
END_TEST

Suite *suite_bmap_getbit(void) {
  Suite *s = suite_create("\033[45m-=BMAP_GETBIT=-\033[0m");
  TCase *tc = tcase_create("bmap_getbit");

  tcase_add_test(tc, bmap_getbit_1);
  tcase_add_test(tc, bmap_getbit_2);
  suite_add_tcase(s, tc);
  return s;
}
