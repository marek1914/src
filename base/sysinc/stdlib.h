void *malloc(size_t size);
void *calloc(size_t nmemb, size_t size);
void *realloc(void *ptr, size_t size);
void free(void *ptr);

void exit(int);
void abort(void);
int atexit(void (*)(void));

char *getenv(const char *);
int putenv(const char *);
int setenv(const char *, const char *, int);
int unsetenv(const char *);
int clearenv(void);

char *mktemp (char *);
int mkstemp (char *);

long strtol(const char *, char **, int);
long long strtoll(const char *, char **, int);
unsigned long strtoul(const char *, char **, int);
unsigned long long strtoull(const char *, char **, int);
double strtod(const char *nptr, char **endptr);

float strtof(const char *nptr, char **endptr);

int atoi(const char *);
long atol(const char *);
long long atoll(const char *);

char *realpath(const char *path, char *resolved);
int system(const char * string);

void *bsearch(const void *key, const void *base0, size_t nmemb, size_t size, int (*compar)(const void *, const void *));
void qsort(void *, size_t, size_t, int (*)(const void *, const void *));

long jrand48(unsigned short *);
long mrand48(void);
long nrand48(unsigned short *);
long lrand48(void);
unsigned short *seed48(unsigned short*);
double erand48(unsigned short xsubi[3]);
double drand48(void);
void srand48(long);
unsigned int arc4random(void);
void arc4random_stir(void);
void arc4random_addrandom(unsigned char *, int);

int rand(void);
void srand(unsigned int seed);
long int random(void);
void srandom(unsigned int seed);

int    unlockpt(int);
char*  ptsname(int);
int    ptsname_r(int, char*, size_t);
int    getpt(void);

int grantpt(int fd);
int      mblen(const char *, size_t);
size_t   mbstowcs(wchar_t *, const char *, size_t);
int      mbtowc(wchar_t *, const char *, size_t);
int	wctomb(char *, wchar_t);
size_t	wcstombs(char *, const wchar_t *, size_t);
