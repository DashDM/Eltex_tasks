#include "bmap_test.h"

START_TEST(bmap_get_len_1) {
  struct bitmap *bm;
  bmap_init(&bm, 11);
  int n = bmap_get_len(bm);
  ck_assert_int_eq(n, 2);
  bmap_delete(&bm);
}
END_TEST

START_TEST(bmap_get_len_2) {
  struct bitmap *bm = NULL;
  int n = bmap_get_len(bm);
  ck_assert_int_eq(n, -1);
}
END_TEST

Suite *suite_bmap_get_len(void) {
  Suite *s = suite_create("\033[45m-=BMAP_GET_LEN=-\033[0m");
  TCase *tc = tcase_create("bmap_get_len");

  tcase_add_test(tc, bmap_get_len_1);
  tcase_add_test(tc, bmap_get_len_2);
  suite_add_tcase(s, tc);
  return s;
}
