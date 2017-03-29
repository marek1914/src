#ifndef  DT_UNKNOWN
#define  DT_UNKNOWN     0
#define  DT_FIFO        1
#define  DT_CHR         2
#define  DT_DIR         4
#define  DT_BLK         6
#define  DT_REG         8
#define  DT_LNK         10
#define  DT_SOCK        12
#define  DT_WHT         14
#endif

struct dirent {
  uint64_t         d_ino;
  int64_t          d_off;
  unsigned short   d_reclen;
  unsigned char    d_type;
  char             d_name[256];
};

typedef struct DIR DIR;

struct dirent* readdir(DIR* dirp);
DIR *opendir(const char *name);
DIR *fdopendir(int fd);
int readdir_r(DIR *dirp, struct dirent *entry, struct dirent **result);
int closedir(DIR *dirp);
void rewinddir(DIR *dirp);
int dirfd(DIR *dirp);
int alphasort(const void *a, const void *b);
int getdents(unsigned int, struct dirent*, unsigned int);
int scandir(const char *dirp, struct dirent ***namelist,
      int (*filter)(const struct dirent *),
      int (*compar)(const struct dirent **, const struct dirent **));

