time_t time(time_t *t);
int nanosleep(const struct timespec *req, struct timespec *rem);

//char *strtotimeval(const char *str, struct timeval *tv);

struct tm {
   int     tm_sec;         /* seconds */
   int     tm_min;         /* minutes */
   int     tm_hour;        /* hours */
   int     tm_mday;        /* day of the month */
   int     tm_mon;         /* month */
   int     tm_year;        /* year */
   int     tm_wday;        /* day of the week */
   int     tm_yday;        /* day in the year */
   int     tm_isdst;       /* daylight saving time */
   long int tm_gmtoff;     /* Seconds east of UTC.  */
   const char *tm_zone;    /* Timezone abbreviation.  */
};

char *asctime(const struct tm *tm);
char *asctime_r(const struct tm *tm, char *buf);


/* Return the difference between TIME1 and TIME0.  */
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

/* global includes */
char*     tzname[];
int       daylight;
long int  timezone;

#define CLOCKS_PER_SEC     1000000

clock_t   clock(void);

#define CLOCK_REALTIME             0
#define CLOCK_MONOTONIC            1
#define CLOCK_PROCESS_CPUTIME_ID   2
#define CLOCK_THREAD_CPUTIME_ID    3
#define CLOCK_REALTIME_HR          4
#define CLOCK_MONOTONIC_HR         5

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

