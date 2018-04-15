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
	int sem_id;
	sem_id=semget(1000,1,0600|IPC_CREAT);
	if(-1==sem_id)
	{
		perror("semget");
		return -1;
	}
	int ret;
	ret=semctl(sem_id,0,SETVAL,1);
	if(-1==ret)
	{
		perror("semctl");
		return -1;
	}
	struct sembuf sopp,sopv;
	sopp.sem_num=0;
	sopp.sem_op=-1;
	sopp.sem_flg=SEM_UNDO;
	sopv.sem_num=0;
	sopv.sem_op=1;
	sopv.sem_flg=SEM_UNDO;
	p[0]=20000000;//创建之前初始化p[0]
	int i;
	//先创建，后连接
	if(!fork())
	{
		for(i=0;i<N;i++)
		{
			semop(sem_id,&sopp,1);//加锁
			p[0]=p[0]-1;
			semop(sem_id,&sopv,1);//解锁
		}
	}else{
		for(i=0;i<N;i++)
		{
			semop(sem_id,&sopp,1);//加锁
			p[0]=p[0]-1;
			semop(sem_id,&sopv,1);//解锁
		}
		wait(NULL);
		printf("p[0]=%d\n",p[0]);
	}
	return 0;
}
