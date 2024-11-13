#include "bmap_test.h"

START_TEST(bmap_reset_1) {
  struct bitmap *bm;

  bmap_init(&bm, 23);

  bm->bits[0] = 238;
  bm->bits[1] = 13;
  bm->bits[2] = 77;

  bmap_reset(bm);

  ck_assert_int_eq(bm->bits[0], 0);
  ck_assert_int_eq(bm->bits[1], 0);
  ck_assert_int_eq(bm->bits[2], 0);

  bmap_delete(&bm);
}
END_TEST

Suite *suite_bmap_reset(void) {
  Suite *s = suite_create("\033[45m-=BMAP_RESET=-\033[0m");
  TCase *tc = tcase_create("bmap_reset");

  tcase_add_test(tc, bmap_reset_1);
  suite_add_tcase(s, tc);
  return s;
}
