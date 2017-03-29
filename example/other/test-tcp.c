#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN 1024*1024
char buf[LEN];

main(void)
{
    struct sockaddr_in saddr;
	int sfd, ret, i;

	for (i=0; i<LEN; i++ ) {
		buf[i] = 'A';
	}
    bzero(&saddr, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htons(INADDR_ANY);
    saddr.sin_port = htons(8082);

    sfd = socket(PF_INET, SOCK_STREAM, 0);

    ret = bind(sfd, (struct sockaddr*)&saddr, sizeof(saddr));
    ret = listen(sfd, 20);

    while (1) {

        struct sockaddr_in client_addr;

        socklen_t length = sizeof(client_addr);

		printf("11\n");
        int new_sfd = accept(sfd, (struct sockaddr*)&client_addr, &length);
       	printf("22 %x \n",client_addr.sin_addr.s_addr);
		send(new_sfd, buf, LEN, 0);
		printf("33\n");
        close(new_sfd);
    }

    close(sfd);
}

