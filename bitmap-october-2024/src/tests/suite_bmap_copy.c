#include "bmap_test.h"

START_TEST(bmap_copy_1) {
  struct bitmap *bm;
  bmap_init(&bm, 8);
  struct bitmap *bm2;
  bmap_init(&bm2, 1);
  bm->bits[0] = 0b11101111;
  bm2->bits[0] = 0b10000000;
  int n = bmap_copy(bm, bm2, 3, 0, 1);
  ck_assert_int_eq(n, 0);
  ck_assert_int_eq(bm->size, 9);
  ck_assert_uint_eq(bm->bits[0], 0b11110111);
  int bit = 0;
  bmap_getbit(bm, 8, &bit);
  ck_assert_int_eq(bit, 1);
  ck_assert_uint_eq(bm->bits[1], 0b10000000);
  bmap_delete(&bm);
  bmap_delete(&bm2);
}
END_TEST

START_TEST(bmap_copy_2) {
  struct bitmap *bm;
  bmap_init(&bm, 8);
  struct bitmap *bm2;
  bmap_init(&bm2, 7);
  bm->bits[0] = 0b11101111;
  bm2->bits[0] = 0b10101010;
  int n = bmap_copy(bm, bm2, 8, 0, 7);
  ck_assert_int_eq(n, 0);
  ck_assert_int_eq(bm->size, 15);
  ck_assert_uint_eq(bm->bits[0], 0b11101111);
  ck_assert_uint_eq(bm->bits[1], bm2->bits[0]);
  bmap_delete(&bm);
  bmap_delete(&bm2);
}
END_TEST

START_TEST(bmap_copy_3) {
  struct bitmap *bm;
  bmap_init(&bm, 8);
  struct bitmap *bm2;
  bmap_init(&bm2, 7);
  bm->bits[0] = 0b11101111;
  bm2->bits[0] = 0b10101010;
  int n = bmap_copy(bm, bm2, 0, 0, 7);
  ck_assert_int_eq(n, 0);
  ck_assert_int_eq(bm->size, 15);
  ck_assert_uint_eq(bm->bits[0], 0b10101011);
  ck_assert_uint_eq(bm->bits[1], 0b11011110);
  bmap_delete(&bm);
  bmap_delete(&bm2);
}
END_TEST

START_TEST(bmap_copy_4) {
  struct bitmap *bm;
  bmap_init(&bm, 8);
  struct bitmap *bm2 = NULL;
  bm->bits[0] = 0b11101111;
  int n = bmap_copy(bm, bm2, 0, 0, 7);
  ck_assert_int_eq(n, FAILURE_CODE);
  bmap_delete(&bm);
}
END_TEST

Suite *suite_bmap_copy(void) {
  Suite *s = suite_create("\033[45m-=BMAP_COPY=-\033[0m");
  TCase *tc = tcase_create("bmap_copy");

  tcase_add_test(tc, bmap_copy_1);
  tcase_add_test(tc, bmap_copy_2);
  tcase_add_test(tc, bmap_copy_3);
  tcase_add_test(tc, bmap_copy_4);
  suite_add_tcase(s, tc);
  return s;
}
