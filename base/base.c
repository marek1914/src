/* think 0-7 or 0-4 */
/* retry */

retry = 0;

do {
	retry ++;
	status = get_status();
} while(!status || retry<8);

do {
	status = get_status();
} while (!status || retry++<8);


retry = 8;
do {
	retry--;
	status = get_status();
} while (!status || retry == 0);

static int controller_busy(void)
{
    int retries = 100;
    unsigned char status;

    do {
        status = inb_p(HD_STATUS);
    } while ((status & BUSY_STAT) && --retries);
    return status;
}

/*
 * C99: ISO/IEC 9899:1999
 * POSIX.1-2001: IEEE 1003.1,2001
 * glibc: GNU C standard library
 * FIXME: TODO: XXX:
 * Usual arithmetic conversions
 * Integer conversion rank
 * Integer promotions
 * debug: printf/gdb/jtag, Each has its own advantage
 * Tip: Uncomment the following line
 */

statfs
linux :0a  
win :0d 0a
// printf("\033[33mHello\033[0m\n"); 打印颜色

/*
//24bit数左移2bit +2 
c->low = (*c->bytestream++)<<18;
c->low+= (*c->bytestream++)<<10;
c->low+= ((*c->bytestream++)<<2) + 2;
*/


/*
example:0x11223344
big-endian: high-low low-high: 11 22 33 44
little-endian: high-high low-low: 44 33 22 11
network byte order = big-endian byte order
middle-endian(^_^): 22 11 44 33 (wikipedia)

Atomic element size 8-bit/16bit (hehe..)
armv6:rev instruction

start from:**
most significant : big
least significant: little
*/

/*
indent:
kernel/uboot tab(*)
ffmpeg  4 space
google  2 space

identifier style:
snake_case: kernel uboot
PascalCase: BackColor
camelCase : backColor
UPPERCASE : IO FLAGS

Class / Enum Pascal
Parameter Camel

C++:
类名PascalCase如 ActionProvider
方法名camelCase如 getString(int column)
类成员变量m(member)+Pascal如 mSurfaceFlingerConsumer
局部变量(包括参数)camelCase如 inputFormat

*/

// Ones' complement
// Two's complement
//-128- -1 0-127
//  80- ff 0-7f

//clang-format -style=Google -dump-config  

/*
Dictionaries:
pictureInfo.aspectRatio
lookupTable
utils.c
serial_core.c
nor_base.c
do_xx()
double-linked list
FILE *fp
FILE *stream
FILE *cfg
int fd
void *ptr
void *dest, const void *src
getCount() 
mCount
eventType
static int array_resize(...)
typedef struct mtd_info nand_info_t;
is_dir()
u32 attr, oldattr;
*/

//等价
 #if !defined (STM32F) 
 #ifndef STM32F
//invalid conversion from 'void*' to 'uint8_t* {aka unsigned char*}'[-fpermissive]
((void(*)(void))((int*)0x80600000))();

static int _do_env_set() //内部函数 _始
__pf // __开头

void *malloc(size_t size);
sizeof //返回size_t
size_t  //stddef.h(gcc中而非libc) aka: unsigned long int
ssize_t //signed size_t sys/types.h aka: long int

void	*priv;	/* pointer to private data	*/

void board_nand_init_tail(struct mtd_info *mtd)
{
	mtd->read = emma_nand_read;
	mtd->write = emma_nand_write;
}

/* for backward compatibility */

/**
 * Base class and low-level protocol for a remotable object.
 */


//左值 lvalue
//类型转换  type cast
//6.3 Conversions


/*
6.8.4.1 The if statement

The controlling expression of an if statement shall have scalar type.
In both forms, the first substatement is executed if the expression compares unequal to 0.
In the else form, the second substatement is executed if the expression compares equal
to 0. If the first substatement is reached via a label, the second substatement is not
executed.
*/

/*
labeled-statement:
identifier : statement
case constant-expression : statement
default : statement
*/

//error: case label does not reduce to an integer constant

(a>b == c>d)
(a>b != c>d)
(a>b || c>d)
(a>b && c>d)

char argv[]; //仅用于形参，正常语句提示需要数组长度
#define MAJOR(dev)	((dev)>>8)
#define MINOR(dev)	((dev) & 0xff)
#define MKDEV(ma,mi)	((ma)<<8 | (mi))

//gid uid

/* The how field should be either SVC_DISABLED or SVC_RESET */
static void service_stop_or_reset(struct service *svc, int how)
{
    /* we are no longer running, nor should we attempt to restart */
    svc->flags &= (~(SVC_RUNNING|SVC_RESTARTING));

    if ((how != SVC_DISABLED) && (how != SVC_RESET)) {
        /* Hrm, an illegal flag.  Default to SVC_DISABLED */
        how = SVC_DISABLED;
    }
    /* if the service has not yet started, prevent it from auto-starting with its class */
    if (how == SVC_RESET) {
        svc->flags |= (svc->flags & SVC_RC_DISABLED) ? SVC_DISABLED : SVC_RESET;
    } else {
        svc->flags |= how;
    }
}



foo().bar; //C函数可返回结构体

struct i2c_dev *i2c_dev;

foo(...)  //fail, at least 1
  
//6.7.1 Storage-class specifiers
//typedef/extern/static/auto/register


error: break statement not within loop or switch
A break statement shall appear only in or as a switch body or loop body
A break statement terminates execution of [the smallest enclosing] switch or iteration statement
A continue statement shall appear only in or as a loop body


//Compound Statement
//error: label at end of compound statement

2种情况：
{
	...
	goto lable;
	...
	lable: //后面没语句了
}

switch (foo) {
	case 1:
	case 2:
		...
	default: //后面没语句		
}
如果真不需要语句，可用空语句";" 如：
想1下跳出2层循环：
while (loop1) {
	while (loop2) {
		if (want_out)
			goto end_loop1;
	}
	end_loop1: ;
}

int: 4
long long: 8
float: 4
double: 8
//x86_64:
long: 8
//arm_32:
long 4

sizeof(3.14)=8 //default double
sizeof(3.14f)=4 //f or F :float

// uint8_t / uint16_t / uint32_t / uint64_t
//posix标准，c99引入
//linaro:libc/usr/include/stdint.h
typedef signed char    int8_t;
typedef unsigned int   uint32_t;

//vla:运行才决定，生命周期内不变
int array1[10];
int const N = 10;
int array2[N];
int n = 10;
int array3[n];

C99:
6.7.5.2 Array declarators

GCC标准规范：
6.19 Arrays of Variable Length ：
作为编译器扩展，GCC编译器遵守 ISO/C99 变长数组规范。


GCC 中零长数组
它作为结构体的最后一个元素非常有用，gcc


char buf[]=
"hello"
"world";

char buf[] = 
"hello\
world";

a=2;b=1;c=3;d=4;
a>b?c>d?1:2:3);

#include <stdbool.h>
return false;

memset(fb_mem, 0, 1024*768);
memset(bdata->node_bootmem_map, 0xff, mapsize); //初始内存位图
memset(bpb, 0, sizeof(*bpb));
memset(dev, 0, sizeof(Dev));

// return structure
struct foo {
	int i;
	int j;
};

struct foo bar(void)
{
	struct foo test;
	test.i=1;
	test.j=2;
	return test;
}

///
void main(void)
{
	goto exit;
	i+=4;
//lable后什么语句都没有不行
//error: label at end of compound statement
exit:
}

//NULL 在stddef.h 中 define

error: expected ‘;’ after struct definition

括号后必须semicolon如：
Thread xxx= new Thread(new Runnable(){
	public void run(){
		for (int i=0; i<10; i++) {
			System.out.println("Hello gaojie!");
		}
	}
}); //here

这如同C：
do {
	...
} while(...); //here

//main(void)后面不能加semicolon，结构体定义后面必须要
struct a {
	char g;
	char f;
};

shell中的 ; 用于分隔语句，原则是如果不会造成歧义就可以不加

char* s="hello";
strstr(s, "ll");
strchr(s, 'l'); //等价 strstr(s, "l");
strrchr(s, 'l')

char* s = "+HTTPACTION: 0, 200, 418, 4096\r\n"
len = atoi(s+=20);
s = strchr(s,',')+1;
len1 = atoi(s);

char buf[]=
"\r\n"
"+HTTPACTION: 0, 200, 1348, 4096\r\n"
"+HTTPACTION: 0, 200, 1348, 4096\r\n"
"+HTTPACTION: 0, 200, 2696, 4096\r\n"
"+HTTPACTION: 0, 200, 4044, 4096\r\n"
"+HTTPACTION: 0, 200, 4318, 4096\r\n"
;

s = strtok(buf, "abc");//abc是单字符非整体
process(...);

while(s = strtok(NULL, "\r\n")) {
	process(...); //要写2遍
}

//改进
for (s = buf;;s = NULL) {
	s = strtok(s, "\r\n") //可以用同一参数 s
	if (!s) break;
	process(...);
}

//无警告
test()
{
	printf("Hello World\n");
}

char buf='\0';

///
void foo(void)
{
	printf("Hello\n");
}

#undef foo
void main(void)
{
	foo();
}
	
char* str1=(void*)0;
if (str==NULL)

//open devices
main()
{
char buf[1024];
buf[1023]=0;
int fd;
fd = open("/sys/kernel/debug/usb/devices", O_RDONLY);
printf("%s \n",strerror(errno));
read(fd,buf,1023);
printf("%s",buf);
}

// # & ##
#define FUNC_INFO(format,args...) printf("hello\n",__FUNCTION__,##args);
#define PIPE_OBJ_CREATE(name)      pipe_##name##_obj_open()
#define LOGD(...) ((void)LOG(LOG_DEBUG, LOG_TAG, __VA_ARGS__))
#define LOG(priority, tag, ...)  LOG_PRI(ANDROID_##priority, tag, __VA_ARGS__)
#define LOG_PRI(priority, tag, ...)  android_printLog(priority, tag, __VA_ARGS__)
#define DFB_INFO_ERR(fmt, arg...)          printf((char *)fmt, ##arg)


/* All data in network byte order */
typedef struct image_header {
	uint32_t	ih_magic;       /* Image Header Magic Number    */
	uint32_t	ih_size;        /* Image Data Size              */
	uint32_t	ih_load;        /* Data  Load  Address          */
	uint32_t	ih_ep;          /* Entry Point Address          */
	uint32_t	ih_dcrc;        /* Image Data CRC Checksum      */
	uint8_t		ih_type;        /* Image Type                   */
	uint8_t		ih_comp;        /* Compression Type             */
	uint8_t		ih_name[32];    /* Image Name           		*/
} image_header_t;


// 调用g++编译器会被定义
#ifdef __cplusplus
#else
#endif

_Bool flag;
flag=1;
flag=false;	
////

//C++
bool flag;


int main(void)
{
/* gcc -mfpu=neon */	
#ifdef __ARM_NEON__
	printf("define\n");
#else
	printf("no define\n");
#endif
}

uint8_t path[128];
strcpy(path, “hello”); //keil 提示类型不匹配问题

long unsigned int
unsigned long int


/*
egl.cfg文件格式
# One line per configuration, of the form:
# D I TAG
# D: display (0: default)
# I: implementation (0: software, 1: hardware)

0 0 android
0 1 POWERVR_SGX_101
*/

Loader::Loader()
{
    FILE* cfg = fopen("egl.cfg", "r");
        while (fgets(line, 256, cfg)) {
    }
}

fd=open()
if (df<0)

// 数组结束靠size或NULL
if (a||b)  if(a|b) //逻辑等价 ^_^


v->cur_seq_no++;
nPage = MIN( itemVisible, itemCount );
barLen = (int) (pageStep * 1.0f/(maxPos - minPos + 1) + 0.5);
bmp_x = (right + left)>>1;  //取中间位置

//	flash擦除页
for (i=0; i<size; i+=0x1000 )
	erase_4k(i);

for (i=0; i<(size+0x1000-1)/0x1000; i++)
	erase_4k( i*0x1000 );

argc: argument count
argv: argument vector
int main(int argc, char *argv[])
int main(int argc, char *argv[], char *envp[])
int main(int argc, char **argv, char **env)
/*
1) char *argv[] 比 char** argv可读性更强，表明argv是字符串数组(UNIX高级编程)
   char *color[] = {"red", "green", "blue"};
2) argc/argv只是形参，可以换成任何名字
3) argv/envp连续，env在栈底，bionic: libc_init_dynamic.c: char** envp = argv+argc+1(下文的nil);
4) exec装载main需3个参数，可部分声明，超出3个或顺序错乱结果不可预知(ABI)
*/

main(int argc, char* argv[])
{
	int i;
	printf("p=%p\n",argv); //!=&argv
	for (i=0; i<10; i++)
		printf("%p %s\n",i, argv[i]);
}

./a.out foo bar foobar

p    =0x7ffdd9489758  //argument vector arra

0x7ffdd948a2a9 ./a.out //'\0'分隔依次排列(同u-boot)
0x7ffdd948a2b1 foo
0x7ffdd948a2b5 bar
0x7ffdd948a2b9 foobar
(nil) (null)   //argv end
0x7ffdd948a2c0 LC_PAPER=en_NZ.UTF-8 // 间隔是"foobar\0" 可见argv和env是连续的
0x7ffdd948a2d5 XDG_VTNR=7
...
0x7ffdd948afcd COLORTERM=gnome-terminal
0x7ffdd948afe6 _=./a.out //接近栈底(cat maps xxxx-xxxx [stack])

//指针
	1) int a[5]; a，&a 都代表首地址
	2) 函数参数中定义不了数组，只是指针变量
		foo(char* s[]){ //char **s 就能迅速判断s!=&s ，这种写法有一定迷惑性
			char* p[5]; // p == &p, s != &s
		}
//structor
    结构体名代表变量本身而非地址(为何总有是地址的错觉，说100遍)
	同类型结构体变量可直接赋值（老式编译器用memcpy()）
	struct foo{int a; int b; };
	struct foo bar = {1,2};
	printf("%d,%d\n", bar); //gcc -> 1,2  因为print函数比较特殊
	test(&bar);

//get file size 

fd = open("foo", O_RDONLY);
len = lseek(fd, 0, SEEK_END);
  	
uint16_t crc; (uint8_t)crc>>8; // 变为0

//无符号有符号的变换  单字节 +- 0x80  双字节 +- 0x8000

//检查空格 \r \n 
while (isspace (*s)) s++;  //ok
//不等价
while (isspace (*s++));    //fail 少了一个字符


printf("pi=%f  %.10f\n", pi); //precision default 6 pi=3.141593
printf("%04hx", x); 
printf("%010.7f", f); // 3.14 -> 03.1400000
printf("Hello\0World\n"); //warning: embedded ‘\0’ in format [-Wformat-contains-nul] ->Hello
printf("Hello\x World\n"); //error: \x used with no following hex digits
printf("%s", 0); //(null)
printf("pi = %.5f\n", 4 * atan(1.0)); // .20f 10几位以后就不对了
//%p as if by %#x or %#lx

const char* const argv[4]
const char*
char * const
char * const *
char * const argv[]

字节拼接：
uint16_t crc;
uint8_t  msg[2];
crc = msg[1]<<8 | msg[0];  //msg[1] 会溢出吗？ 

struct stat statbuf;  
stat(filename,&statbuf);  
int size=statbuf.st_size; 

do {
	property_get("mstar.bootvideo.sync", url, "0");
	usleep(50*1000);
} while (url[0] == '0');


main(int argc, char** argv)
{
	int i=0;
	while (argv[i++] != NULL) { //打印的是i=1 隐藏的 bug
		printf("%s %d W", argv[i], i);	
	}
}

goto: 可减少缩进
j=-i;

/* "static" means don't export the symbol */
static void foo(void)
{
}

//取http数据长度
p = strtok(buf, "\r\n");

if (strcmp(p, "HTTP/1.1 200 OK")) {
    printf("fail\n");
    return;
} else {
    printf("head ok\n");
}

do {
    p = strtok(NULL, "\r\n");
    token = strstr(p, "Content-Length: ");
} while (!token && p);

if (token) {
    *contentLen = atoi(token + 16);
}
//

// glibc

char *strsep(char **stringp, const char *delim);   
应该是string separate  delimiter:分隔符

stdarg.h :standard arguments   // man stdarg
/* ISO C Standard:  7.15  Variable arguments  <stdarg.h>*/
定义依赖平台， va_list就是void*

char *strchr(const char *s,char c);  查找字符串s中首次出现字符c的位置
strrchr. 取得某字符最后出现处起的字符串。
char *strstr(const char *haystack, const char *needle); //locate a substring

puts(""); //打印 \n

stream 是字节序列的抽象概念

#include <sys/types.h>
#include <dirent.h>

DIR *opendir(const char *name);

char *fgets(char *s, int n, FILE *stream);//read n-1

研究busybox中ls.c文件：
ls -d *s

if (!argv[0])
	*--argv = (char*)".";
若ls命令后无参数，则指当前路径。

getcwd/getwd : 取得目前所在目录 

//同一字符串两地址相减得到长度
root_len = (rest - path);

feof 宏

for(i = 0; i < 256; i++)
printf("errno.%02d is: %s/n", i, strerror(i));  打印所有错误号的解释

打开错误返回-1，判断可以
if (fd == -1)   or
if (fd < 0)

warning: format ‘%d’ expects argument of type ‘int’, but argument 2 has type ‘ssize_t’
  printf("ret = %d\n", ret);
解决：%ld

检查文件是否存在并可读
失败返回-1 （-1作为if条件，也是非零? 即-1是逻辑1）
if (access(path, R_OK)) {
    continue;
}

----
处理一行一行的东西时，利用返回的NULL判断结束
如openmax定义接口的时候，index到返回空的时候，标示结束

-----
FILE *fp;
fp = fopen("...", "w");
if (!fp)
    return 1;

FILE *file = fopen(buf, "r");
if (file == NULL)
    return false;
---
if (file)
	return false;
---
if (file != NULL)
	return false;

logcat 错误：
+++ LOG: write failed (errno=28)
strerror(28) -> No space left on device

qsort 函数，使用

offsetof(stddef.h中定义)

memcpy
strcpy

pow()
sqrt()


unistd.h  unix standard head 包含unix标准调用如read write getpid close
standard input output header  stdio.h 
standard library header  stdlib.h 

#include <sys/socket.h>

sys/types.h

int 4字节 long 机器长度

fcntl.h定义了open,fcntl函数,感官上open/close/read/write应该在一起，
但open的man页中有很多fnctl字样，这2个函数有着千丝万缕的联系，
且open时可以“控制”很多文件的属性。所以open不在unistd.h中。
fcntl()的使用到底没搞明白。

有时（如bionic）只引用fcntl.h，不引用unistd.h也能调用read，write，是因为前者包含了后者。bionic中还对此包含加注释：
/* this is not required, but makes client code much happier */

linaro编译器中（编译器中带着glibc以及系统的所有头文件）

linaro编译器中 fcntl.h 简单说只定义了 fcntl open create，并有注释：POSIX Standard: 6.5 File Control Operations

x86:
/usr/include/sys/有：
socket.h
types.h
stat.h
ioctl.h
仅定义ioctl函数，bionic在unistd.h中也声明ioctl函数不标准

/usr/include/：
stdio.h
stdlib.h
string.h

//第一个C标准由ANSI发布(ANSI-C)，后被ISO采纳
//ISO标准:C89/C90/C99/C11(ISO/IEC 9899:2011)

bool:
C++: 原生支持
C99:
stdbool.h (在gcc编译器中)
#define bool _Bool
#define false 0
#define true 1

//在编译器和bionic/libc/kernel/common/linux/stddef.h中定义了：
#  if !defined(__cplusplus)
#   define NULL ((void*)0)
#  else
#   define NULL (0)

下面文件包含 stddef.h
stdint.h
unistd.h
string.h
stdlib.h
stdio.h

typeof是gcc扩展操作符，检查数据类型
int x;
typeof(x) y;

//stddef.h定义
#define offsetof(type, member) ((size_t) &((type *)0)->member)

#define container_of(ptr, type, member) \ 
    typeof(((type *)0)->member) *__mptr = ptr; \
    (type *)((char *)__mptr - offsetof(type, member));

EOF:end-of-file, 值因平台而异，glibc stdio.h定义为 -1

libevent 同步事件通知库
gevent

<inttypes.h> (C99 7.8)
"Range: bytes=%"PRId64"-" (PRIdN   print SCNdN   scan)
escape character(转义字符^_^)
escape sequence
\nnn 1-3bit octal (2bit octal max 63)
\xHH 1-2bit hexadecimal

是否加0占位取决于后面字符如：
"hello\12world\n" //不用加0，因为w无法解释到八进制
"hello\122world\n"
"hello\0122world\n"//不加0占位会解释为\122
'\0' 也是\nnn形式
-----

/*
epoll - I/O event notification facility
epoll是改进的poll，是多路复用IO接口 select/poll 的增强版
*/

Why is open declared in fcntl.h, while close is declared in unistd.h?
I presume this is a historical artifact, but this pair strikes 
me as especially odd since you often need to close after open.
As previously pointed out, sockets and pipes also get passed to close. But probably 
the original reason is that open needs flags like O_RDONLY that are defined in fcntl.h, 
so you might as well put the prototype in that file.

#include <fcntl.h> //定义open,fcntl  
#include <unistd.h> //POSIX定义的UNIX系统服务函数如read、write和getpid

用fwrite写文件时，会有2G大小限制。加编译选项-D_FILE_OFFSET_BITS=64解决

stat, fstat, lstat - get file status

//makedev, major, minor - manage a device number

void klog_init(void)
{
    const char *name = "/dev/__kmsg__";
    if (mknod(name, S_IFCHR | 0600, (1 << 8) | 11) == 0) { 
    //1,11 是/dev/kmsg 这样做是多此一举？！
        klog_fd = open(name, O_WRONLY);
        fcntl(klog_fd, F_SETFD, FD_CLOEXEC);  
        // fork可以访问这个fd，但执行了exec就不可以用这个fd了
        unlink(name); 
    }
}

#define LOG_BUF_MAX 512

void klog_write(int level, const char *fmt, ...)
{
    char buf[LOG_BUF_MAX];
    va_list ap;

    if (level > klog_level) return;
    if (klog_fd < 0) return;

    va_start(ap, fmt);
    vsnprintf(buf, LOG_BUF_MAX, fmt, ap);
    buf[LOG_BUF_MAX - 1] = 0;
    va_end(ap);
    write(klog_fd, buf, strlen(buf));
}


//取参数
if (sscanf(l, "%d %d %d", &width, &height, &fps) == 3) {
...
} else if (sscanf(l, " %c %d %d %s", &pathType, &count, &pause, path) == 4) {
...
}
//能正常解析
1280 720 4 5
p 0 0 part0

//不能解析
1280 720 4
9 0 0 part0


/* Low level event handling module */
if (is_xxx)
else

if (!is_xxx)
else
	

struct Token {
  const unsigned char *z; /* Text of the token.  Not NULL-terminated! */
  unsigned dyn  : 1;      /* True for malloced memory, false for static */
  unsigned n    : 31;     /* Number of characters in this token */
};

#include <sys/socket.h>

struct sockaddr_un {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 sa_family_t sun_family;
 char sun_path[UNIX_PATH_MAX];
};

AF_UNIX 在init 以及adb 有用到

getsockname - get socket name

这样调用：       
caddr.sin_family = AF_INET;
caddr.sin_port = htons(9090); 
caddr.sin_addr.s_addr = 0;

bind(sfd,(struct sockaddr*)&caddr,sizeof(caddr));

strace后发现：

bind(3, {sa_family=AF_INET, sin_port=htons(9090), sin_addr=inet_addr("0.0.0.0")}, 16)
sendto(3, "\0\1\0\10!\22\244B\257\35\17\344\312\201\376!\0\0\0\0\0\3\0\4\0\0\0\0", 28, ...) 中间的数据是8进制表示。

stun中，用随机数生成transid

sock=socket(PF_PACKET, SOCK_RAW, htons(ETH_P_IP)) 必须root权限才能成功， 内核如何检查？

在建立PF_PACKET族的socket时会调用packet_create,

在packet_create中会检查用户是否有CAP_NET_RAW的capability：

if (!capable(CAP_NET_RAW))
    return -EPERM;
root拥有所有的capability,

linux socket buffer默认8k
/proc/sys/net/core
/proc/sys/net/ipv4/tcp_wmem  4096(4k) 16384(16k)  4194304(4M)
/proc/sys/net/ipv4/tcp_rmem  4096(4k) 87380(80+ k)    6291456(6M)
/proc/sys/net/ipv6

setsockopt 设定socket属性包括buffer大小

int socket(int domain, int type, int protocol);
domain: 通讯域 选择通讯协议，常见(有的是PF_ ?)：
AF_UNIX, AF_LOCAL   Local communication (windows(or ce)不支持，进程通讯使用INET 127.0.0.1)
AF_INET             IPv4 Internet protocols
AF_NETLINK          Kernel user interface device

type: specifies the communication semantics 常见：
SOCK_STREAM  提供sequenced, reliable, two-way, connection-based 字节流，支持out-of-band传输机制
SOCK_DGRAM   datagrams (connectionless, unreliable messages of a fixed maximum length)

从Linux 2.6.27开始，type除了指定类型，还可以OR上如下参数改变socket行为：
SOCK_NONBLOCK  默认是阻塞模式，设置为非阻塞
SOCK_CLOEXEC

SOCK_STREAM类型Sockets是full-duplex字节流，类似于pipes。

client 端向server端发起connect请求

shutdown(socketfd) - shut down part of a full-duplex connection
close(socketfd) 关闭socket连接

bind : bind a name to a socket，就是给新建socket赋地址(如IP+端口)，叫做： “assigning a name to a socket”.
listen : listen for connections on a socket
int listen(int sockfd, int backlog);
sockfd 只能是 SOCK_STREAM 或 SOCK_SEQPACKET类型

accept :accept a connection on a socket(服务端，用于SOCK_STREAM)，参数addr保存客户端的，它创建一个新的socket


OMX_IndexParamVideoProfileLevelQuerySupported 检索：

do{
	eRet = OMX_GetParameter(Handle,OMX_IndexParamVideoProfileLevelQuerySupported,&profileLevel);
	profileLevel.nProfileIndex++;
	printf("**eLevel = %lx,eLevel.eProfile = %lx \n",profileLevel.eLevel,profileLevel.eProfile);
}while(eRet!=OMX_ErrorNoMore);

得到：
**eLevel = 1,eLevel.eProfile = 1 
**eLevel = 2,eLevel.eProfile = 1 
**eLevel = 4,eLevel.eProfile = 1 
**eLevel = 8,eLevel.eProfile = 1 


do_gettimeofday

char* fb_mem;
fb = open ("/dev/fb0", O_RDWR);
fb_mem = mmap(NULL, 1024*768, PROT_READ|PROT_WRITE,MAP_SHARED,fb,0);

waiting ? 1 : 0;


==thread==
libutils/Threads.cpp

Derived class implement threadLoop().  
There are 2 ways of using the Thread object:
1) loop: if threadLoop() returns true
2) once: if threadLoop() returns false

virtual bool threadLoop() = 0;

创建的线程是 _threadLoop(void* user) 调threadLoop() 
res = createThreadEtc(_threadLoop, this, name, priority, stack, &mThread);
this指针传到 pthread_create 的arg参数,即_threadLoop(void* user) 的user

//一个函数设置标志，另一个函数检查标志
//下一函数const不可定义可以理解，这个为啥还能被继承？应该也是const的吧
virtual void requestExit();
bool exitPending() const;

// vim: tw=80
