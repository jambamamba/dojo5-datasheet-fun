#pragma once
#ifdef __cplusplus
extern "C" {
#endif


#include <stdio.h>

#define LOG(format, ...) \
    debuglogger_log(__FILE__, __LINE__, format __VA_OPT__(,) __VA_ARGS__);

void debuglogger_log(const char* file, int line_num, const char *format, ...);

#ifdef __cplusplus
}
#endif
