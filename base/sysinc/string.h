void *memccpy(void *dest, const void *src, int c, size_t n);
void *memchr(const void *s, int c, size_t n);
void *memrchr(const void *s, int c, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);
void *memcpy(void *dest, const void *src, size_t n);
void *memmove(void *dest, const void *src, size_t n);
void *memset(void *s, int c, size_t n);
void *memmem(const void *haystack, size_t haystacklen, const void *needle, size_t needlelen);

char *index(const char *s, int c);
char *rindex(const char *s, int c);

char *strchr(const char *s, int c);
char *strrchr(const char *s, int c);

size_t strlen(const char *s);
int strcmp(const char *s1, const char *s2);
char *strcpy(char *dest, const char *src);
char *strcat(char *dest, const char *src);
int strcasecmp(const char *s1, const char *s2);

int strncasecmp(const char *s1, const char *s2, size_t n);
char *strdup(const char *s);
char *strstr(const char *haystack, const char *needle);
char *strcasestr(const char *haystack, const char *needle);
char *strtok(char *str, const char *delim);
char *strtok_r(char *str, const char *delim, char **saveptr);

char *strerror(int errnum);
int strerror_r(int errnum, char *buf, size_t buflen);

size_t strnlen(const char *s, size_t maxlen);
char *strncat(char *dest, const char *src, size_t n);

char *strdup(const char *s);

char*  strndup(const char *, size_t);
int    strncmp(const char *, const char *, size_t);
char*  strncpy(char* , const char* , size_t);
size_t strlcat(char* , const char* , size_t);
size_t strlcpy(char* , const char* , size_t);
size_t strcspn(const char *, const char *);
char*  strpbrk(const char *, const char *);
char*  strsep(char** , const char* );
size_t strspn(const char *, const char *);
char*  strsignal(int  sig);
int    strcoll(const char *, const char *);
size_t strxfrm(char* , const char* , size_t);

