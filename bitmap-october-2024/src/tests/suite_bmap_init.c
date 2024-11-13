#include "bmap_test.h"

START_TEST(bmap_init_1) {
  struct bitmap *bm;
  int n = bmap_init(&bm, 11);
  ck_assert_int_eq(n, 0);
  ck_assert_int_eq(bm->size, 11);
  bmap_delete(&bm);
}
END_TEST

START_TEST(bmap_init_2) {
  struct bitmap *bm = NULL;
  int n = bmap_init(&bm, 0);
  ck_assert_int_eq(n, FAILURE_CODE);
  ck_assert_ptr_eq(bm, NULL);
}
END_TEST

Suite *suite_bmap_init(void) {
  Suite *s = suite_create("\033[45m-=BMAP_INIT=-\033[0m");
  TCase *tc = tcase_create("bmap_init");

  tcase_add_test(tc, bmap_init_1);
  tcase_add_test(tc, bmap_init_2);
  suite_add_tcase(s, tc);
  return s;
}
