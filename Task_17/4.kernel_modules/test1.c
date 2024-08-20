#include <linux/module.h>
#include <linux/kernel.h>

int init_module(void) {
    pr_info("LEEEEEROOOOOY\n");
    return 0;
}

void cleanup_module(void) {
    pr_info  ("JEEEEENKIIIINS\n");
}

MODULE_LICENSE("GPL");
