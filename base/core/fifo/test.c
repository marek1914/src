#include <stdio.h>
#include <string.h>
#include "fifo.h"

int main(void)
{
	char s[] = "helloworldzhaojiajia";
	char chr, chr1;
	int i;

	FifoBuffer *fifo = fifo_alloc(128);

	printf("space is :%d\n", fifo_space(fifo));

	fifo_generic_write(fifo, s, strlen(s), NULL);
	printf("space is :%d\n", fifo_space(fifo));

	for (i = 0; fifo_size(fifo)>0; i++) {
		chr1 = 'S';
		if (fifo_size(fifo)>2) {
			fifo_generic_peek_at(fifo, &chr1, 2, 1, NULL);
		}
		fifo_generic_read(fifo, &chr, 1, NULL);
		if (i>50) {
			break;
		}
		printf("%c -- %c\n", chr, chr1);
	}
	return 0;
}
