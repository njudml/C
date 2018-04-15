#include "func.h"

int main(int argc,char* argv[])
{
	int shmid;
	shmid=shmget(1000,4096,0600|IPC_CREAT);
	if(-1==shmid)
	{
		perror("shmget");
		return -1;
	}
	int *p;
	p=(int*)shmat(shmid,NULL,0);
	if((int*)-1==p)
	{
		perror("shmat");
		return -1;
	}
	printf("p[0]=%d\n",p[0]);
	while(1);
	return 0;
}
