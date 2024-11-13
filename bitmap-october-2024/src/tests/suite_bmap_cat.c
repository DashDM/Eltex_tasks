#include "bmap_test.h"

START_TEST(bmap_cat_1) {
  struct bitmap *bm;
  bmap_init(&bm, 8);
  struct bitmap *bm2;
  bmap_init(&bm2, 8);
  bm->bits[0] = 0b01010101;
  bm2->bits[0] = 0b10101010;
  struct bitmap *result = NULL;
  result = bmap_cat(bm, bm2);
  ck_assert_ptr_ne(result, NULL);
  ck_assert_uint_eq(result->size, 16);
  ck_assert_uint_eq(result->bits[0], 0b01010101);
  ck_assert_uint_eq(result->bits[1], 0b10101010);
  bmap_delete(&bm);
  bmap_delete(&bm2);
  bmap_delete(&result);
}
END_TEST

START_TEST(bmap_cat_2) {
  struct bitmap *bm = NULL;
  struct bitmap *bm2;
  bmap_init(&bm2, 8);
  bm2->bits[0] = 0b10101010;
  struct bitmap *result;
  result = bmap_cat(bm, bm2);
  ck_assert_ptr_eq(result, NULL);
  bmap_delete(&bm2);
}
END_TEST

START_TEST(bmap_cat_3) {
  struct bitmap *bm;
  bmap_init(&bm, 7);
  struct bitmap *bm2;
  bmap_init(&bm2, 8);
  bm->bits[0] = 0b01010100;
  bm2->bits[0] = 0b10101010;
  struct bitmap *result = NULL;
  result = bmap_cat(bm, bm2);
  ck_assert_ptr_ne(result, NULL);
  ck_assert_uint_eq(result->size, 15);
  ck_assert_uint_eq(result->bits[0], 0b01010101);
  ck_assert_uint_eq(result->bits[1], 0b01010100);
  bmap_delete(&bm);
  bmap_delete(&bm2);
  bmap_delete(&result);
}
END_TEST

Suite *suite_bmap_cat(void) {
  Suite *s = suite_create("\033[45m-=BMAP_CAT=-\033[0m");
  TCase *tc = tcase_create("bmap_cat");

  tcase_add_test(tc, bmap_cat_1);
  tcase_add_test(tc, bmap_cat_2);
  tcase_add_test(tc, bmap_cat_3);
  suite_add_tcase(s, tc);
  return s;
}
