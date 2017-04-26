#ifndef _LOG_H
#define _LOG_H

#define LOG_QUIET   -1
#define LOG_ERROR    0
#define LOG_WARNING  1
#define LOG_INFO     2
#define LOG_VERBOSE  3
#define LOG_DEBUG    4

void av_log(int level, const char *fmt, ...);
void log_set_level(int level);

#endif
