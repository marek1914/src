#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

struct stat {
   dev_t     st_dev;     /* ID of device containing file */
   ino_t     st_ino;     /* inode number */
   mode_t    st_mode;    /* protection */
   nlink_t   st_nlink;   /* number of hard links */
   uid_t     st_uid;     /* user ID of owner */
   gid_t     st_gid;     /* group ID of owner */
   dev_t     st_rdev;    /* device ID (if special file) */
   off_t     st_size;    /* total size, in bytes */
   blksize_t st_blksize; /* blocksize for filesystem I/O */
   blkcnt_t  st_blocks;  /* number of 512B blocks allocated */
   time_t    st_atime;   /* time of last access */
   time_t    st_mtime;   /* time of last modification */
   time_t    st_ctime;   /* time of last status change */
};
           
char **environ;
__noreturn void _exit(int);

pid_t  fork(void);
pid_t  vfork(void);
pid_t  getpid(void);
pid_t  gettid(void);
pid_t  getpgid(pid_t);
int    setpgid(pid_t, pid_t);
pid_t  getppid(void);
pid_t  getpgrp(void);
int    setpgrp(void);
pid_t  setsid(void);

int execv(const char *, char * const *);
int execvp(const char *, char * const *);
int execve(const char *, char * const *, char * const *);
int execl(const char *, const char *, ...);
int execlp(const char *, const char *, ...);
int execle(const char *, const char *, ...);
int capget(cap_user_header_t hdrp, cap_user_data_t datap);
int capset(cap_user_header_t hdrp, const cap_user_data_t datap);

int prctl(int  option, ...);

int nice(int);

int setuid(uid_t);
uid_t getuid(void);
int seteuid(uid_t);
uid_t geteuid(void);
int setgid(gid_t);
gid_t getgid(void);
int setegid(gid_t);
gid_t getegid(void);
int getgroups(int, gid_t *);
int setgroups(size_t, const gid_t *);
int setreuid(uid_t, uid_t);
int setregid(gid_t, gid_t);
int setresuid(uid_t, uid_t, uid_t);
int setresgid(gid_t, gid_t, gid_t);
int getresuid(uid_t *ruid, uid_t *euid, uid_t *suid);
int getresgid(gid_t *rgid, gid_t *egid, gid_t *sgid);
int issetugid(void);
char* getlogin(void);
char* getusershell(void);
void setusershell(void);
void endusershell(void);



/* Macros for access() */
#define R_OK  4  /* Read */
#define W_OK  2  /* Write */
#define X_OK  1  /* Execute */
#define F_OK  0  /* Existence */

int access(const char *, int);
int link(const char *, const char *);
int unlink(const char *);
int chdir(const char *);
int fchdir(int);
int rmdir(const char *);
int pipe(int *);
#ifdef _GNU_SOURCE  /* GLibc compatibility */
int pipe2(int *, int);
#endif
int chroot(const char *);
int symlink(const char *, const char *);
int readlink(const char *, char *, size_t);
int chown(const char *, uid_t, gid_t);
int fchown(int, uid_t, gid_t);
int lchown(const char *, uid_t, gid_t);
int truncate(const char *, off_t);
char *getcwd(char *, size_t);

int sync(void);

int close(int);
off_t lseek(int fd, off_t offset, int whence);
off64_t lseek64(int fd, off64_t offset, int whence);

ssize_t read(int, void *, size_t);
ssize_t write(int, const void *, size_t);
ssize_t pread(int, void *, size_t, off_t);
ssize_t pwrite(int, const void *, size_t, off_t);

int dup(int);
int dup2(int, int);
int fcntl(int, int, ...);
int ioctl(int, int, ...);
int flock(int, int);
int fsync(int);
int fdatasync(int);
int ftruncate(int, off_t);

int pause(void);
unsigned int alarm(unsigned int);
unsigned int sleep(unsigned int);
int usleep(unsigned long);

int gethostname(char *, size_t);

int getdtablesize(void);

void *__brk(void *);
int brk(void *);
void *sbrk(ptrdiff_t);

int getopt(int, char * const *, const char *);
char *optarg;
int optind, opterr, optopt;

int isatty(int);
char* ttyname(int);
int ttyname_r(int, char*, size_t);

int  acct(const char*  filepath);
int sysconf(int  name);
int daemon(int, int);
int cacheflush(long start, long end, long flags);
pid_t tcgetpgrp(int fd);
int   tcsetpgrp(int fd, pid_t _pid);

int stat(const char *path, struct stat *buf);
int fstat(int fd, struct stat *buf);
int lstat(const char *path, struct stat *buf);
