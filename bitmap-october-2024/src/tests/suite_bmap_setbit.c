#include "bmap_test.h"

START_TEST(bmap_setbit_1) {
  struct bitmap *bm;
  int bit = 0;

  bmap_init(&bm, 17);

  bmap_setbit(bm, 5, 1);
  bmap_getbit(bm, 5, &bit);

  ck_assert_int_eq(bit, 1);

  bmap_setbit(bm, 15, 1);
  bmap_setbit(bm, 14, 0);
  bmap_setbit(bm, 13, 1);

  ck_assert_int_eq(bm->bits[1], 5);

  bmap_delete(&bm);
}
END_TEST

START_TEST(bmap_setbit_2) {
  struct bitmap *bm = NULL;
  bmap_init(&bm, 4);
  int code = bmap_setbit(bm, 5, 1);
  ck_assert_int_eq(code, FAILURE_CODE);
  bmap_delete(&bm);
}
END_TEST

Suite *suite_bmap_setbit(void) {
  Suite *s = suite_create("\033[45m-=BMAP_SETBIT=-\033[0m");
  TCase *tc = tcase_create("bmap_setbit");

  tcase_add_test(tc, bmap_setbit_1);
  tcase_add_test(tc, bmap_setbit_2);
  suite_add_tcase(s, tc);
  return s;
}
