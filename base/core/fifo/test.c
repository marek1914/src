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
	printf("space is :%d size if %d\n", fifo_space(fifo), fifo_size(fifo));

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


/* test */
#ifdef TEST
int main(void)
{
    int i, j, n;

    /* create a FIFO buffer */
    FifoBuffer *fifo = fifo_alloc(13 * sizeof(int));

    /* fill data */
    for (i = 0; fifo_space(fifo) >= sizeof(int); i++)
        fifo_generic_write(fifo, &i, sizeof(int), NULL);

    /* peek at FIFO */
    n = fifo_size(fifo) / sizeof(int);
    for (i = -n + 1; i < n; i++) {
        int *v = (int *)fifo_peek2(fifo, i * sizeof(int));
        printf("%d: %d\n", i, *v);
    }
    printf("\n");

    /* peek_at at FIFO */
    n = fifo_size(fifo) / sizeof(int);
    for (i = 0; i < n; i++) {
        fifo_generic_peek_at(fifo, &j, i * sizeof(int), sizeof(j), NULL);
        printf("%d: %d\n", i, j);
    }
    printf("\n");

    /* read data */
    for (i = 0; fifo_size(fifo) >= sizeof(int); i++) {
        fifo_generic_read(fifo, &j, sizeof(int), NULL);
        printf("%d ", j);
    }
    printf("\n");

    /* test *ndx overflow */
    fifo_reset(fifo);
    fifo->rndx = fifo->wndx = ~(uint32_t)0 - 5;

	printf("-----\n");
    /* fill data */
    for (i = 0; fifo_space(fifo) >= sizeof(int); i++)
        fifo_generic_write(fifo, &i, sizeof(int), NULL);

    /* peek_at at FIFO */
    n = fifo_size(fifo) / sizeof(int);
    for (i = 0; i < n; i++) {
        fifo_generic_peek_at(fifo, &j, i * sizeof(int), sizeof(j), NULL);
        printf("%d: %d\n", i, j);
    }

    fifo_free(fifo);

    return 0;
}
#endif
