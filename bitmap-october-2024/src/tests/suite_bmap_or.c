#include "bmap_test.h"

START_TEST(bmap_or_1) {
  struct bitmap *bm1;
  struct bitmap *bm2;

  bmap_init(&bm1, 16);
  bmap_init(&bm2, 8);

  bm1->bits[0] = 0b00111100;
  bm1->bits[1] = 0b11110000;

  bm2->bits[0] = 0b10100001;

  struct bitmap *res = bmap_or(bm1, bm2);

  ck_assert_int_eq(res->bits[0], 0b00111100);
  ck_assert_int_eq(res->bits[1], 0b11110001);

  bmap_delete(&bm1);
  bmap_delete(&bm2);
  bmap_delete(&res);
}
END_TEST

START_TEST(bmap_or_2) {
  struct bitmap *bm1;
  struct bitmap *bm2;

  bmap_init(&bm1, 8);
  bmap_init(&bm2, 8);

  bm1->bits[0] = 0b00001110;
  bm2->bits[0] = 0b11100000;

  struct bitmap *res = bmap_or(bm1, bm2);

  ck_assert_int_eq(res->bits[0], 0b11101110);

  bmap_delete(&bm1);
  bmap_delete(&bm2);
  bmap_delete(&res);
}
END_TEST

START_TEST(bmap_or_3) {
  struct bitmap *bm1;
  struct bitmap *bm2;

  bmap_init(&bm1, 23);
  bmap_init(&bm2, 15);

  bm1->bits[1] = 0b01011000;

  bm2->bits[0] = 0b10000101;

  struct bitmap *res = bmap_or(bm2, bm1);

  ck_assert_int_eq(res->bits[0], 0b00000000);
  ck_assert_int_eq(res->bits[1], 0b11011101);
  ck_assert_int_eq(res->bits[2], 0b00000000);

  bmap_delete(&bm1);
  bmap_delete(&bm2);
  bmap_delete(&res);
}
END_TEST

START_TEST(bmap_or_4) {
  struct bitmap *bm1;
  struct bitmap *bm2 = NULL;

  bmap_init(&bm1, 23);

  bm1->bits[1] = 0b01011000;

  struct bitmap *res = bmap_or(bm2, bm1);

  ck_assert_ptr_eq(res, NULL);

  bmap_delete(&bm1);
}
END_TEST

Suite *suite_bmap_or(void) {
  Suite *s = suite_create("\033[45m-=BMAP_OR=-\033[0m");
  TCase *tc = tcase_create("bmap_or");

  tcase_add_test(tc, bmap_or_1);
  tcase_add_test(tc, bmap_or_2);
  tcase_add_test(tc, bmap_or_3);
  tcase_add_test(tc, bmap_or_4);
  suite_add_tcase(s, tc);
  return s;
}
