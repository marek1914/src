#include <stdio.h>
#include <string.h>

char buf[]=
	"\r\n"
	"+HTTPACTION: 0, 200, 1348, 4096\r\n"
	"+HTTPACTION: 0, 200, 1348, 4096\r\n"
	"+HTTPACTION: 0, 200, 1348, 4096\r\n"
	;

void main(void)
{
	char *s;
	int len, len1;


	for (s = buf;; s = NULL) {
		s = strtok(s, "\r\n");
		if (!s) break;
		len = atoi(s+=20);
		s = strchr(s,',')+1;
		len1 = atoi(s);
	}

	printf("%d %d\n", len, len1);
}
