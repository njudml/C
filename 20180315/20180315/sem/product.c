#include "func.h"
//创建一个信号量集合，里边含有两个信号量
int main()
{
	int sem_id;
	sem_id=semget(1000,2,0600|IPC_CREAT);
	if(-1==sem_id)
	{
		perror("semget");
		return -1;
	}
	unsigned short arr[2]={10,0};//第一个信号量代表仓库大小，第二个代表产品个数
	int ret;
	ret=semctl(sem_id,0,SETALL,arr);
	if(-1==ret)
	{
		perror("semctl");
		return -1;
	}
	bzero(arr,sizeof(arr));
	ret=semctl(sem_id,0,GETALL,arr);
	if(-1==ret)
	{
		perror("semctl");
		return -1;
	}
	printf("arr[0]=%d,arr[1]=%d\n",arr[0],arr[1]);
	struct sembuf sopp,sopv;
	sopp.sem_num=0;
	sopp.sem_op=-1;
	sopp.sem_flg=SEM_UNDO;
	sopv.sem_num=1;
	sopv.sem_op=1;
	sopv.sem_flg=SEM_UNDO;
	printf("I am a producter\n");
	while(1)
	{
		printf("space left=%d,product num=%d\n",semctl(sem_id,0,GETVAL),semctl(sem_id,1,GETVAL));
		semop(sem_id,&sopp,1);
		printf("producting\n");
		semop(sem_id,&sopv,1);
		printf("space left=%d,product num=%d\n",semctl(sem_id,0,GETVAL),semctl(sem_id,1,GETVAL));
		sleep(1);
	}
	return 0;
}
