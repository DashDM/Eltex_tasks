#include "bmap_test.h"

START_TEST(bmap_and_1) {
  struct bitmap *bm;
  struct bitmap *bm2;
  bmap_init(&bm, 16);
  bm->bits[0] = 0b11111111;
  bm->bits[1] = 0b11111111;

  bmap_init(&bm2, 16);
  bm->bits[0] = 0b10101010;
  bm->bits[1] = 0b10101010;

  struct bitmap *res = bmap_and(bm, bm2);
  ck_assert_ptr_ne(res, NULL);

  ck_assert_int_eq(bmap_is_eq(res, bm2), 1);

  bmap_delete(&bm);
  bmap_delete(&bm2);
  bmap_delete(&res);
}
END_TEST

START_TEST(bmap_and_2) {
  struct bitmap *bm;
  struct bitmap *bm2;
  bmap_init(&bm, 7);
  bm->bits[0] = 0b11111110;

  bmap_init(&bm2, 16);
  bm2->bits[0] = 0b11111111;
  bm2->bits[1] = 0b11111111;

  struct bitmap *res = bmap_and(bm, bm2);
  ck_assert_ptr_ne(res, NULL);

  ck_assert_uint_eq(res->bits[0], 0b00000000);
  ck_assert_uint_eq(res->bits[1], 0b01111111);

  bmap_delete(&bm);
  bmap_delete(&bm2);
  bmap_delete(&res);
}
END_TEST

START_TEST(bmap_and_3) {
  struct bitmap *bm;
  struct bitmap *bm2;
  bmap_init(&bm, 7);
  bm->bits[0] = 0b11111110;

  bmap_init(&bm2, 5);
  bm2->bits[0] = 0b10101000;

  struct bitmap *res = bmap_and(bm, bm2);
  ck_assert_ptr_ne(res, NULL);

  ck_assert_uint_eq(res->bits[0], 0b00101010);

  bmap_delete(&bm);
  bmap_delete(&bm2);
  bmap_delete(&res);
}
END_TEST

START_TEST(bmap_and_4) {
  struct bitmap *bm;
  struct bitmap *bm2 = NULL;
  bmap_init(&bm, 7);
  bm->bits[0] = 0b11111110;

  struct bitmap *res = bmap_and(bm, bm2);
  ck_assert_ptr_eq(res, NULL);

  bmap_delete(&bm);
}
END_TEST

Suite *suite_bmap_and(void) {
  Suite *s = suite_create("\033[45m-=BMAP_AND=-\033[0m");
  TCase *tc = tcase_create("bmap_and");

  tcase_add_test(tc, bmap_and_1);
  tcase_add_test(tc, bmap_and_2);
  tcase_add_test(tc, bmap_and_3);
  tcase_add_test(tc, bmap_and_4);
  suite_add_tcase(s, tc);
  return s;
}
