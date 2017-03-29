#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


int main(int argc, char **argv)
{
	int sfd;
	ssize_t ret;
	struct sockaddr_in saddr, caddr;
	char msg[65509] = {0,1,0,8,0x21,0x12,0xa4,0x42,0x94,0x13,0x99,1, 0x80,0x2f,\
					0xe0,0xeb, 0, 0, 0, 0, 0, 3, 0, 4, 0, 0, 0, 0, 0};
	sfd = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&caddr, 0, sizeof(caddr)); 
    caddr.sin_family = AF_INET;
	caddr.sin_port = htons(9090); 
    caddr.sin_addr.s_addr = 0;
    
    bind(sfd,(struct sockaddr*)&caddr,sizeof(caddr));

    
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
	inet_aton("218.205.48.254",&saddr.sin_addr);
    saddr.sin_port = htons(37026); //Huawei stun server
 
    ret = sendto(sfd, msg, 65508, 0, (struct sockaddr*)&saddr, sizeof(saddr));
	printf("ret = %d errno=%s\n", ret, strerror(errno));
//sendto(sfd, msg, 28, 0, (struct sockaddr*)&saddr, sizeof(saddr));
//sendto(sfd, msg, 28, 0, (struct sockaddr*)&saddr, sizeof(saddr));



    close(sfd);
    return 0;
}
