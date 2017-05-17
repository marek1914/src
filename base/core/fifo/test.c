#include <stdio.h>
#include <string.h>
#include "fifo.h"

int main(void)
{
	char s[] = "hello world";
	char chr;
	int i;

	FifoBuffer *fifo = fifo_alloc(128);

	printf("space is :%d\n", fifo_space(fifo));

	fifo_generic_write(fifo, s, strlen(s), NULL);
	printf("space is :%d\n", fifo_space(fifo));

	for (i = 0; i < 14; i++) {
		fifo_generic_read(fifo, &chr, 1, NULL);
		printf("%c\n", chr);
	}
	return 0;
}
