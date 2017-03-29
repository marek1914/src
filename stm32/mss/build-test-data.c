#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


uint32_t data[256];

int main(void)
{
	int i, j;
	int count[8] = {12, 9, 7, 19, 20, 25, 29, 14};

	for (i = 0; i < 8; i++) {
		data[i*32] = count[i];
		for (j = 0; j < count[i]; j++) {
			data[i*32+1+j] = random()%2048<<19 | random()%2048<<8 | random()%256;
		}
	}

	for (i = 0; i < 256; i++) {
		if (!(i % 8)) {
			puts("");
		}
		printf("0x%08x, ", data[i]);
	}
	puts("");
	
	return 0;
}
