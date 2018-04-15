#include "func.h"

int main(int argc,char* argv[])
{
	if(argc!=2)
	{
		printf("error args\n");
		return -1;
	}
	key_t key;
	key=ftok(argv[1],1);
	printf("%d\n",key);
	int shmid;
	shmid=shmget(key,4096,0600|IPC_CREAT);
	if(-1==shmid)
	{
		perror("shmget");
		return -1;
	}
	return 0;
}
