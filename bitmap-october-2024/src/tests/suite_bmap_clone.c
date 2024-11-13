#include "bmap_test.h"

START_TEST(bmap_clone_1) {
  struct bitmap *bm;
  bmap_init(&bm, 11);
  struct bitmap *bm2 = bmap_clone(bm);
  ck_assert_int_eq(bm->size, bm2->size);
  ck_assert_int_eq(bmap_is_eq(bm, bm2), 1);
  bmap_delete(&bm);
  bmap_delete(&bm2);
}
END_TEST

START_TEST(bmap_clone_2) {
  struct bitmap *bm = NULL;
  struct bitmap *bm2 = bmap_clone(bm);
  ck_assert_ptr_eq(bm2, NULL);
}
END_TEST

Suite *suite_bmap_clone(void) {
  Suite *s = suite_create("\033[45m-=BMAP_CLONE=-\033[0m");
  TCase *tc = tcase_create("bmap_clone");

  tcase_add_test(tc, bmap_clone_1);
  tcase_add_test(tc, bmap_clone_2);
  suite_add_tcase(s, tc);
  return s;
}
