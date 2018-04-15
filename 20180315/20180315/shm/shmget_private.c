#include "func.h"

int main(int argc,char* argv[])
{
	int shmid;
	shmid=shmget(IPC_PRIVATE,4096,0600|IPC_CREAT);
	if(-1==shmid)
	{
		perror("shmget");
		return -1;
	}
	return 0;
}
