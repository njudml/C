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
	int ret;
	ret=shmctl(shmid,IPC_RMID,NULL);//标记删除，最后一个进程解除连接，就会被删除
	if(-1==ret)
	{
		perror("shmctl");
		return -1;
	}
	return 0;
}
