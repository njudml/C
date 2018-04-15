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
	unsigned short arr[2]={5,10};
	int ret;
//	ret=semctl(sem_id,0,SETALL,arr);
//	if(-1==ret)
//	{
//		perror("semctl");
//		return -1;
//	}
//	bzero(arr,sizeof(arr));
//	ret=semctl(sem_id,0,GETALL,arr);
//	if(-1==ret)
//	{
//
//			perror("semctl");
//		return -1;
//	}
//	printf("arr[0]=%d,arr[1]=%d\n",arr[0],arr[1]);
//	return 0;
}
