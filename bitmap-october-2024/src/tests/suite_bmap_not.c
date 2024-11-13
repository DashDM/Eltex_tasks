#include "bmap_test.h"

START_TEST(bmap_not_1) {
  struct bitmap *bm;

  bmap_init(&bm, 21);

  bm->bits[0] = 0b10011001;
  bm->bits[1] = 0b01010010;
  bm->bits[2] = 0b11011000;

  bmap_not(bm);

  ck_assert_int_eq(bm->bits[0], 0b01100110);
  ck_assert_int_eq(bm->bits[1], 0b10101101);
  ck_assert_int_eq(bm->bits[2], 0b00100000);

  bmap_delete(&bm);
}
END_TEST

START_TEST(bmap_not_2) {
  struct bitmap *bm = NULL;

  int code = bmap_not(bm);

  ck_assert_int_eq(code, FAILURE_CODE);
}
END_TEST

Suite *suite_bmap_not(void) {
  Suite *s = suite_create("\033[45m-=BMAP_NOT=-\033[0m");
  TCase *tc = tcase_create("bmap_not");

  tcase_add_test(tc, bmap_not_1);
  tcase_add_test(tc, bmap_not_2);
  suite_add_tcase(s, tc);
  return s;
}
