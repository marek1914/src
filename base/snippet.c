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


// vim: tw=80 
