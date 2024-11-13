#include "common.h"

void get_current_time(char *buffer, size_t buffer_size) {
  time_t rawtime;
  struct tm *timeinfo;

  time(&rawtime);
  timeinfo = localtime(&rawtime);

  strftime(buffer, buffer_size, "[%Y-%m-%d %H:%M:%S]", timeinfo);
}

void log_msg(const char *message, ...) {
  char time_buffer[30];
  get_current_time(time_buffer, sizeof(time_buffer));
  
  char msg[log_msg_size] = { 0 };
  va_list args;
  va_start(args, message);
  vsnprintf(msg, log_msg_size, message, args);
  va_end(args);
  
#ifdef LOG_TO_FILE
  FILE *log_file = fopen("log.txt", "a");
  if (log_file == NULL) {
    fprintf(stderr, "Error opening log file: %s\n", strerror(errno));
  } else {
    fprintf(log_file, "%s %s\n", time_buffer, msg);
    fclose(log_file);
  }
#endif

#ifdef LOG_TO_STDERR
  fprintf(stderr, "%s %s\n", time_buffer, msg);
#endif

#ifdef LOG_TO_SYSLOG
  openlog("Bitmap", LOG_PID | LOG_CONS, LOG_USER);
  syslog(LOG_INFO, "%s", msg);
  closelog();
#endif
}
