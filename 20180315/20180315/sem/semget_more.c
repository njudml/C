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
	return 0;
}
