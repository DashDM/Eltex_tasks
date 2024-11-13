#ifndef COMMON_H
#define COMMON_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <time.h>
#include <stdarg.h>

#define log_msg_size 256

void log_msg(const char *message, ...);
void get_current_time(char *buffer, size_t buffer_size);

#endif
