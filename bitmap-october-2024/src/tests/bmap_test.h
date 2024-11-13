#ifndef BMAP_TEST_H
#define BMAP_TEST_H

#include <check.h>
#include "../lib/bitmap.h"

Suite* suite_bmap_init(void);
Suite* suite_bmap_delete(void);
Suite* suite_bmap_resize(void);
Suite* suite_bmap_comparison(void);
Suite* suite_bmap_clone(void);
Suite* suite_bmap_copy(void);

Suite* suite_bmap_setbit(void);
Suite* suite_bmap_getbit(void);

Suite* suite_bmap_and(void);

Suite* suite_bmap_reset(void);
Suite* suite_bmap_cat(void);

Suite* suite_bmap_or(void);
Suite* suite_bmap_xor(void);
Suite* suite_bmap_not(void);

Suite* suite_bmap_get_len(void);

void run_testcase(Suite* testcase);
void run_tests(void);
#endif
