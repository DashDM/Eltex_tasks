#include "bmap_test.h"

START_TEST(bmap_resize_1) {
  struct bitmap *bm;
  bmap_init(&bm, 11);
  int n = bmap_resize(bm, 3);
  ck_assert_int_eq(n, 0);
  ck_assert_int_eq(bm->size, 3);
  bmap_delete(&bm);
}
END_TEST

Suite *suite_bmap_resize(void) {
  Suite *s = suite_create("\033[45m-=BMAP_RESIZE=-\033[0m");
  TCase *tc = tcase_create("bmap_resize");

  tcase_add_test(tc, bmap_resize_1);
  suite_add_tcase(s, tc);
  return s;
}
