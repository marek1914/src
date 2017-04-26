#include <stdarg.h>
#include <stdio.h>
#include "log.h"

static int log_level = LOG_DEBUG;

void av_log(int level, const char *fmt, ...)
{   /* error warn info ver debug */ 
	unsigned char color[5] = {1, 3, 4, 2, 6};
    va_list vl;
    va_start(vl, fmt);
    
    if (level > log_level)
        return;

    if (level != LOG_INFO) {
		printf("\033[3%dm", color[level]);
		vprintf(fmt, vl);
		printf("\033[0m");
    } else
        vprintf(fmt, vl);
    va_end(vl);
}

void log_set_level(int level)
{
    log_level = level;
}

