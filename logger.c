#include "logger.h"

#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>


#define LINE_SZ 1024

static long currentTimeMillis() 
{
  struct timeval time;
  gettimeofday(&time, NULL);

  return (time.tv_sec * 1000 + time.tv_usec / 1000) % 1000;
}
static char *getTimeStamp(char *time_stamp, size_t time_stamp_len) {
    time_t rawtime;
    struct tm *info;
    time( &rawtime );
    info = localtime( &rawtime );
    strftime(time_stamp, time_stamp_len,"%x-%I:%M:%S%p", info);
    int sz = snprintf(NULL, 0, "%s.%li", time_stamp, currentTimeMillis());
    if(sz >= time_stamp_len){
        fprintf(stdout, "time_stamp line is too long at %i bytes, max line length limit is %i\n", sz, (int)time_stamp_len-1);
        abort();
    }
    snprintf(time_stamp + strlen(time_stamp), time_stamp_len - strlen(time_stamp) - 1, ".%li", currentTimeMillis());
    return time_stamp;
}

void debuglogger_log(const char* file, int line_num, const char *format, ...)
{
  char time_stamp[80] = {0};
  getTimeStamp(time_stamp, sizeof(time_stamp));
  char buffer[LINE_SZ];
  va_list args;
  va_start (args, format);
  int sz = snprintf(NULL, 0, "[%s] [LINE:%i] ",
    time_stamp,
    line_num
    );
  if (sz >= sizeof(buffer)) {
    fprintf(stderr, "log line is too long at %i bytes, max line length limit is %i\n", sz, (int)sizeof(buffer)-1);
    abort();
  }
  snprintf(buffer, sizeof(buffer), "[%s] [LINE:%i] ",
    time_stamp,
    line_num
    );
  vsnprintf (buffer + strlen(buffer), sizeof(buffer) - strlen(buffer), format, args);
  va_end (args);

  printf("%s\n", buffer);
}