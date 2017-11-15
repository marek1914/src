#define eprintf(...) fprintf (stderr, __VA_ARGS__) //c99
#define eprintf(args...) fprintf (stderr, args)
#define INFO(fmt, a...)  printf(fmt, ##a)
#define INFO(fmt, ...)  printf(fmt, ##__VA_ARGS__) //## 无第二参时起作用

/* 意义不大 */
#define VERSION_MAIN   (0x02U)
#define VERSION_SUB1   (0x06U)
#define VERSION_SUB2   (0x01U)
#define VERSION_RC     (0x00U) /* release candidate */
#define VERSION        ((VERSION_MAIN << 24)|(VERSION_SUB1 << 16)\
                       |(VERSION_SUB2 << 8 )|(VERSION_RC))

#if defined(foo)
#elif defined(bar)
#else
#endif

#if !defined (STM32F405xx) && !defined (STM32F415xx)
#endif

#if defined(STM32F405xx) || defined(STM32F415xx)
#endif

#if defined(STM32F405xx)
#include "stm32f405xx.h"
#elif defined(STM32F415xx)
#include "stm32f415xx.h"
#elif defined(STM32F413xx)
#include "stm32f413xx.h"
#else
#error "Please select the target"

flag = 0b11001100; //gcc扩展，非c99

/* 代码检查侵入太多 */
int sys_init(void)
{
	int ret;
	ret = a_init(void);
	if (ret < 0)
		return -1;
	ret = b_init(void);
	if (ret < 0)
		return -1;
	ret = c_init(void);
	if (ret < 0)
		return -1;
	return 0;
}

/* 非主流 */
int sys_init(void)
{
	int ret = 0;
	ret += a_init(void);
	ret += b_init(void);
	ret += c_init(void);
	if (ret == 0)
		return 0;
	else
		return -1;
}

/* 几种返回值处理方法 */

if (ioctl(fd, FBIOGET_FSCREENINFO, &finfo) == -1)
	return -errno;

if (ioctl(s, SIOCGIWPRIV, &wrq) < 0) {
    return -1;
}

r = ioctl(ifc_ctl_sock, SIOCGIFHWADDR, &ifr);
if(r < 0)
	return -1;

r = ioctl(usb->desc, USBDEVFS_CLAIMINTERFACE, &interface);
if(r != 0) goto fail;

/* ---------------- */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#if !foo || !bar
#error "foobar"
#endif

if (stat != 0)  // equivalent to if (stat)
if (stat == 0)  // equivalent_to if (!stat)
if (fp == NULL) // equivalent_to if (!fp)
if (fp != NULL) // more readable

//bootanimation，count=0 无限循环，r益出也无所谓
for (int r=0 ; !part.count || r<part.count ; r++) {
}

/* 原位置删除空格 */
i = j = 0;
while (s[i]) {
	if (s[i] != ' ') {
		s[j] = s[i];
		j++;
	}
	i++;
}
s[j] = '\0';

/* 另起位置删除空格 */
while (*p)
	if (*p == ' ')
		p++;
	else
		*q++ = *p++;
*q = '\0';

/* 删除空格同时转大写 */
while (*p)
	if (*p == ' ')
		p++;
	else
		*(q)++ = tolower(*(p)++);
*q = '\0';

/* 适配器 */
static int parse_msg_type(const char *str)
{
	if (!strcmp(str, "error"))
		return FSCK_ERROR;
	else if (!strcmp(str, "warn"))
		return FSCK_WARN;
	else if (!strcmp(str, "ignore"))
		return FSCK_IGNORE;
	else
		die("Unknown fsck message type: '%s'", str);
}

//超时+标志判断
cnt = 200;
while (cnt > 0 && Flag == 0xFF) {
	sleep(100); 
	cnt--;
}

for (cnt=0; i<200 && Flag == 0xFF; cnt++) {
	sleep(100);
}

/* c要么等于要么不等于 */
if (c=='s') {

} else {

}

//container_of
#define container_of(ptr, type, member) ({  \
  const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
  (type *)( (char *)__mptr - offsetof(type,member) );})

#define container_of(ptr, type, member) ({  \
  (type *)( (char *)ptr - offsetof(type,member) );})
  
/* alpha混合 0全透 ff不透 */
dR = ((sR-dR)*A>>8)+dR;	
dG = ((sG-dG)*A>>8)+dG;	
dB = ((sB-dB)*A>>8)+dB;

void uart_print(char *p)
{
	while (*p) {
		uart_byte(p++);
	}
	uart_byte("\n");
}

/* 返回毫秒 */
int64_t av_gettime_relative(void)
{
    struct timespec ts;
    /* CLOCK_REALTIME */
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (int64_t)ts.tv_sec * 1000000 + ts.tv_nsec / 1000;
}

/* clock_t times(); */

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

int printf(const char *fmt, ...)
{
    int ret;
    va_list ap; 

    va_start(ap, fmt);
    ret = vfprintf(stdout, fmt, ap);
    va_end(ap);
    return (ret);
}


/*
extract token from string
strtok() 标准 连续delim不返回
strsep() 非标 连续delim中间返回NULL
如：  "hello#@world","#@"
strtok调用2次，分别返回hello， world
strsep调用3此，分别返回hello，NULL，world
可见strsep更适合处理单字符delim
*/

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

/* --------------*/
/*extract token from string
strtok() /strsep()
*/

s = strtok(buf, "abc");
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

/* ------------ */
char param[] = "id=100&name=iot&space=beijing&type=open";
char *s = param;
char *s1, *s2;

while (s1 = strsep(&s, "&")) {
	while (s2 = strsep(&s1, "=")) {
		printf("%s\n", s2);
	}
	printf("--------\n");
}

// vim: tw=80 
