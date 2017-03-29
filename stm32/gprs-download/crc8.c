#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

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

int main(int argc, char* argv[])
{
	unsigned int crc;
	unsigned char *p;
	int fd;
	off_t len;
	ssize_t count;

	fd = open(argv[1], O_RDONLY);
	
	if (fd < 0) {
		return 0;
	}

	len = lseek(fd, 0, SEEK_END);
	p = malloc(len);

	lseek(fd, 0, SEEK_SET);
	count = read(fd, p, len);
	printf("%02x\n", crc8(0, p, len));
	//printf("%c", crc8(0, p, len));
	
	return 0;
}


