#include "bmap_test.h"

START_TEST(bmap_is_greater1) {
  struct bitmap *bm1;
  struct bitmap *bm2;

  // bm1 = 110100
  // bm2 = 000100000010100
  bmap_init(&bm1, 6);
  bmap_init(&bm2, 15);

  bmap_setbit(bm1, 0, 1);
  bmap_setbit(bm1, 1, 1);
  bmap_setbit(bm1, 3, 1);

  bmap_setbit(bm2, 3, 1);
  bmap_setbit(bm2, 10, 1);
  bmap_setbit(bm2, 12, 1);

  int n = bmap_is_greater(bm1, bm2);
  ck_assert_int_eq(n, 0);

  n = bmap_is_greater(bm2, bm1);
  ck_assert_int_eq(n, 1);

  bmap_delete(&bm1);
  bmap_delete(&bm2);
}
END_TEST

START_TEST(bmap_is_greater2) {
  struct bitmap *bm1;
  struct bitmap *bm2;

  // bm1 = 111100
  // bm2 = 000000000110100
  bmap_init(&bm1, 6);
  bmap_init(&bm2, 15);

  bmap_setbit(bm1, 0, 1);
  bmap_setbit(bm1, 1, 1);
  bmap_setbit(bm1, 2, 1);
  bmap_setbit(bm1, 3, 1);

  bmap_setbit(bm2, 9, 1);
  bmap_setbit(bm2, 10, 1);
  bmap_setbit(bm2, 12, 1);

  int n = bmap_is_greater(bm1, bm2);
  ck_assert_int_eq(n, 1);

  n = bmap_is_greater(bm2, bm1);
  ck_assert_int_eq(n, 0);

  bmap_setbit(bm1, 2, 0);

  n = bmap_is_greater(bm1, bm2);
  ck_assert_int_eq(n, 0);

  n = bmap_is_greater(bm2, bm1);
  ck_assert_int_eq(n, 0);

  bmap_delete(&bm1);
  bmap_delete(&bm2);
}
END_TEST

START_TEST(bmap_is_greater3) {
  struct bitmap *bm1;
  struct bitmap *bm2;

  // bm1 = 110100
  // bm2 = 000000000110100
  bmap_init(&bm1, 6);
  bmap_init(&bm2, 15);

  bmap_setbit(bm1, 0, 1);
  bmap_setbit(bm1, 1, 1);
  bmap_setbit(bm1, 3, 1);

  bmap_setbit(bm2, 9, 1);
  bmap_setbit(bm2, 10, 1);
  bmap_setbit(bm2, 12, 1);

  int n = bmap_is_greater(bm2, bm1);
  ck_assert_int_eq(n, 0);

  n = bmap_is_greater(bm1, bm2);
  ck_assert_int_eq(n, 0);

  bmap_delete(&bm1);
  bmap_delete(&bm2);
}
END_TEST

// --------------------

START_TEST(bmap_is_eq1) {
  struct bitmap *bm1;
  struct bitmap *bm2;

  bmap_init(&bm1, 5);
  bmap_init(&bm2, 6);
  bm1->bits[0] = 0b00010000;
  bm2->bits[0] = 0b00011000;

  int n = bmap_is_eq(bm1, bm2);
  ck_assert_int_eq(n, 0);

  bmap_delete(&bm1);
  bmap_delete(&bm2);
}
END_TEST

START_TEST(bmap_is_eq2) {
  struct bitmap *bm1;
  struct bitmap *bm2;

  // bm1 = 110100
  // bm2 = 000000000110100
  bmap_init(&bm1, 6);
  bmap_init(&bm2, 15);

  bmap_setbit(bm1, 0, 1);
  bmap_setbit(bm1, 1, 1);
  bmap_setbit(bm1, 3, 1);

  bmap_setbit(bm2, 9, 1);
  bmap_setbit(bm2, 10, 1);
  bmap_setbit(bm2, 12, 1);

  int n = bmap_is_eq(bm2, bm1);
  ck_assert_int_eq(n, 1);

  n = bmap_is_eq(bm1, bm2);
  ck_assert_int_eq(n, 1);

  bmap_setbit(bm2, 2, 1);

  n = bmap_is_eq(bm2, bm1);
  ck_assert_int_eq(n, 0);

  n = bmap_is_eq(bm1, bm2);
  ck_assert_int_eq(n, 0);

  bmap_delete(&bm1);
  bmap_delete(&bm2);
}
END_TEST

// -------

START_TEST(bmap_is_less1) {
  struct bitmap *bm1;
  struct bitmap *bm2;

  bmap_init(&bm1, 6);
  bmap_init(&bm2, 15);
  bm1->bits[0] = 5;
  bm2->bits[0] = 4;
  bm2->bits[1] = 7;

  int n = bmap_is_less(bm2, bm1);
  ck_assert_int_eq(n, 0);

  n = bmap_is_less(bm1, bm2);
  ck_assert_int_eq(n, 1);

  bmap_delete(&bm1);
  bmap_delete(&bm2);
}
END_TEST

START_TEST(bmap_is_less_or_eq1) {
  struct bitmap *bm1;
  struct bitmap *bm2;

  bmap_init(&bm1, 6);
  bmap_init(&bm2, 15);
  bm1->bits[0] = 5;
  bm2->bits[0] = 4;
  bm2->bits[1] = 7;

  int n = bmap_is_less_or_eq(bm2, bm1);
  ck_assert_int_eq(n, 0);

  n = bmap_is_less_or_eq(bm1, bm2);
  ck_assert_int_eq(n, 1);

  bmap_delete(&bm1);
  bmap_delete(&bm2);
}
END_TEST

START_TEST(bmap_is_less_or_eq2) {
  struct bitmap *bm1;
  struct bitmap *bm2;

  // bm1 = 110100
  // bm2 = 000000000010100
  bmap_init(&bm1, 6);
  bmap_init(&bm2, 15);

  bmap_setbit(bm1, 0, 1);
  bmap_setbit(bm1, 1, 1);
  bmap_setbit(bm1, 3, 1);

  bmap_setbit(bm2, 10, 1);
  bmap_setbit(bm2, 12, 1);

  int n = bmap_is_less_or_eq(bm2, bm1);
  ck_assert_int_eq(n, 1);

  n = bmap_is_less_or_eq(bm1, bm2);
  ck_assert_int_eq(n, 0);

  bmap_delete(&bm1);
  bmap_delete(&bm2);
}
END_TEST

// ------

START_TEST(bmap_is_greater_or_eq1) {
  struct bitmap *bm1;
  struct bitmap *bm2;

  bmap_init(&bm1, 6);
  bmap_init(&bm2, 15);
  bm1->bits[0] = 5;
  bm2->bits[0] = 4;
  bm2->bits[1] = 7;

  int n = bmap_is_greater_or_eq(bm2, bm1);
  ck_assert_int_eq(n, 1);

  n = bmap_is_greater_or_eq(bm1, bm2);
  ck_assert_int_eq(n, 0);

  bmap_delete(&bm1);
  bmap_delete(&bm2);
}
END_TEST

START_TEST(bmap_is_greater_or_eq2) {
  struct bitmap *bm1;
  struct bitmap *bm2;

  bmap_init(&bm1, 6);
  bmap_init(&bm2, 15);
  bm1->bits[0] = 5;
  bm2->bits[0] = 0;
  bm2->bits[1] = 5;

  int n = bmap_is_greater_or_eq(bm2, bm1);
  ck_assert_int_eq(n, 1);

  n = bmap_is_greater_or_eq(bm1, bm2);
  ck_assert_int_eq(n, 0);

  bmap_delete(&bm1);
  bmap_delete(&bm2);
}
END_TEST

Suite *suite_bmap_comparison(void) {
  Suite *s = suite_create("\033[45m-=BMAP_COMPARISON=-\033[0m");
  TCase *tc = tcase_create("bmap_compare");

  tcase_add_test(tc, bmap_is_greater1);
  tcase_add_test(tc, bmap_is_greater2);
  tcase_add_test(tc, bmap_is_greater3);
  tcase_add_test(tc, bmap_is_eq1);
  tcase_add_test(tc, bmap_is_eq2);
  tcase_add_test(tc, bmap_is_less1);
  tcase_add_test(tc, bmap_is_less_or_eq1);
  tcase_add_test(tc, bmap_is_less_or_eq2);
  tcase_add_test(tc, bmap_is_greater_or_eq1);
  tcase_add_test(tc, bmap_is_greater_or_eq2);
  suite_add_tcase(s, tc);
  return s;
}
