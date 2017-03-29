#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

main()
{
char buf[1024];
buf[1023]=0;
int fd;
fd = open("./data/avcc.264", O_RDONLY);
printf("%s \n",strerror(errno));
read(fd,buf,1023);
printf("%s",buf);
}
