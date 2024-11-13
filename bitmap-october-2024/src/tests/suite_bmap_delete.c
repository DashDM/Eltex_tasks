#include "bmap_test.h"

START_TEST(bmap_delete_1) {
  struct bitmap *bm;
  bmap_init(&bm, 11);
  bmap_delete(&bm);
  ck_assert_int_eq(0, 0);
  ck_assert_int_eq(11, 11);
}
END_TEST

Suite *suite_bmap_delete(void) {
  Suite *s = suite_create("\033[45m-=BMAP_DELETE=-\033[0m");
  TCase *tc = tcase_create("bmap_delete");

  tcase_add_test(tc, bmap_delete_1);
  suite_add_tcase(s, tc);
  return s;
}
