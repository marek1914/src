time_t time(time_t *t);
int nanosleep(const struct timespec *req, struct timespec *rem);

//char *strtotimeval(const char *str, struct timeval *tv);

struct tm {
   int     tm_sec;
   int     tm_min;
   int     tm_hour;
   int     tm_mday;
   int     tm_mon;
   int     tm_year;
   int     tm_wday;
   int     tm_yday;
   int     tm_isdst;
   long int tm_gmtoff;
   const char *tm_zone;
};

char *asctime(const struct tm *tm);
char *asctime_r(const struct tm *tm, char *buf);
double difftime(time_t time1, time_t time0);
time_t mktime(struct tm *tm);
struct tm *localtime(const time_t *timep);
struct tm *localtime_r(const time_t *timep, struct tm *result);
struct tm *gmtime(const time_t *timep);
struct tm *gmtime_r(const time_t *timep, struct tm *result);
char *strptime(const char *s, const char *format, struct tm *tm);
size_t strftime(char *s, size_t max, const char *format, const struct tm *tm);
char *ctime(const time_t *timep);
char *ctime_r(const time_t *timep, char *buf);
void  tzset(void);
char*     tzname[];
int       daylight;
long int  timezone;
clock_t   clock(void);

#define CLOCK_REALTIME
#define CLOCK_MONOTONIC
#define CLOCK_PROCESS_CPUTIME_ID
#define CLOCK_THREAD_CPUTIME_ID
#define CLOCK_REALTIME_HR
#define CLOCK_MONOTONIC_HR

int timer_create(clockid_t clockid, struct sigevent *sevp, timer_t *timerid);
int timer_delete(timer_t timerid);
int timer_settime(timer_t timerid, int flags, const struct itimerspec *new_value, struct itimerspec * old_value);
int timer_gettime(timer_t timerid, struct itimerspec *curr_value);
int timer_getoverrun(timer_t timerid);
int clock_getres(clockid_t clk_id, struct timespec *res);
int clock_gettime(clockid_t clk_id, struct timespec *tp);
int clock_settime(clockid_t clk_id, const struct timespec *tp);
int gettimeofday(struct timeval *tv, struct timezone *tz);
int settimeofday(const struct timeval *tv, const struct timezone *tz);
