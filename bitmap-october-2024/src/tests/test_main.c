#include "bmap_test.h"

void run_tests(void) {
  Suite *list_cases[] = {suite_bmap_init(),    suite_bmap_delete(),
                         suite_bmap_resize(),  suite_bmap_comparison(),
                         suite_bmap_clone(),   suite_bmap_setbit(),
                         suite_bmap_getbit(),  suite_bmap_reset(),
                         suite_bmap_and(),     suite_bmap_copy(),
                         suite_bmap_xor(),     suite_bmap_or(),
                         suite_bmap_not(),     suite_bmap_cat(),
                         suite_bmap_get_len(), NULL};
  for (Suite **current_testcase = list_cases; *current_testcase != NULL;
       ++current_testcase) {
    run_testcase(*current_testcase);
  }
}

void run_testcase(Suite *testcase) {
  static int count_testcase = 1;
  if (count_testcase > 0) {
    printf("\n");
    count_testcase++;
  }
  SRunner *sr = srunner_create(testcase);

  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_NORMAL);
  srunner_free(sr);
}

int main(void) {
  run_tests();
  return 0;
}
