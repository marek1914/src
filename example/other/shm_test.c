#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
typedef struct{
	char name[4];
	int age;
} people;
main(int argc, char** argv)
{
	int shm_id,i;
	key_t key;
	char temp;
	people *p_map;
	char* name = "/home/gaojie/NXP/shm_test";
	key = ftok(name,0);
	if(key==-1)
		perror("ftok error");

	printf("HaHa key=%x\r\n",key);
	shm_id=shmget(key,4096,IPC_CREAT);	
	printf("Test shm_id= %d\r\n",shm_id);
	if(shm_id==-1)
	{
		perror("shmget error");
		return;
	}
	p_map=(people*)shmat(shm_id,NULL,0);
	
	temp='a';
	printf("TTT %x %d\r\n",p_map,errno);
	for(i = 0;i<10;i++)
	{
		temp+=1;
		memcpy((*(p_map+i)).name,&temp,1);
		(*(p_map+i)).age=20+i;
	}
	printf("RRR\r\n");
//	if(shmdt(p_map)==-1)
//		perror(" detach error ");
sleep(100);
}

