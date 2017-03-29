#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

int  main( int argc, char ** argv )
{
int i;
printf("argc is %d \r\n",argc);
	for( i=0;i<10;i++)
		printf("%s\r\n",argv[i]);
    uid_t ruid, euid;

    printf( "sizeof(uid_t) = %d  sizeof(gid_t) = %d\n", sizeof(uid_t), sizeof(gid_t) );

    ruid = getuid();
    euid = geteuid();
    printf("Start: ruid=%d euid=%d\n", ruid, euid);

    if (seteuid(9999) != 0)
       perror("seteuid(9999)");

    ruid = getuid();
    euid = geteuid();
    printf("After set: ruid=%d euid=%d\n", ruid, euid);

    if (seteuid(0) != 0)
       perror("seteuid(0)");

    ruid = getuid();
    euid = geteuid();
    printf("After restore: ruid=%d euid=%d\n", ruid, euid);

    return 0;
}


