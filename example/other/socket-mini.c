#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char **argv)
{
	int sfd;
	struct sockaddr_in saddr, caddr;

    sfd = socket(AF_INET,SOCK_STREAM,0);

    memset(&caddr, 0, sizeof(caddr)); 
    caddr.sin_family = AF_INET;
	caddr.sin_port = 0x8223;
    caddr.sin_addr.s_addr = htons(INADDR_ANY);
    

    //bind(sfd,(struct sockaddr*)&caddr,sizeof(caddr));

    
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
	inet_aton("192.168.7.32",&saddr.sin_addr);
    saddr.sin_port = 20480;
 
    connect(sfd,(struct sockaddr*)&saddr, sizeof(saddr));

    close(sfd);
    return 0;
}
