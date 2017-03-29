#include <stdio.h>

#define POLY	(0x1070U << 3)

unsigned char _crc8(unsigned short data)
{
	int i;

	for (i = 0; i < 8; i++) {
		if (data & 0x8000)
			data = data ^ POLY;
		data = data << 1;
	}

	return (unsigned char)(data >> 8);
}

unsigned int crc8(unsigned int crc, const unsigned char *vptr, int len)
{
	int i;

	for (i = 0; i < len; i++)
		crc = _crc8((crc ^ vptr[i]) << 8);

	return crc;
}

int main(void)
{
	unsigned int crc;
	unsigned char p[16] = {0,2,3,4,5,6,7,8,9,10,11,12,13,14,15,7};
	//初始值可以随意指定
	crc = crc8(0, p,16);
	printf("crc=0x%x \n", crc);

	crc = crc8(0, p, 8);
	printf("crc1=0x%x\n", crc);
	crc = crc8(crc, p+8, 8);
	printf("crc2=0x%x\n", crc);
	//分多次运算会跟一次运算得出同样结果
	return 0;
}


