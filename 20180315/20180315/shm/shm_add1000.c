#include "func.h"

#define N 10000000
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
	p[0]=0;//创建之前初始化p[0]
	int i;
	//先创建，后连接
	if(!fork())
	{
		for(i=0;i<N;i++)
		{
			p[0]=p[0]+1;
		}
	}else{
		for(i=0;i<N;i++)
		{
			p[0]=p[0]+1;
		}
		wait(NULL);
		printf("p[0]=%d\n",p[0]);
	}
	return 0;
}
