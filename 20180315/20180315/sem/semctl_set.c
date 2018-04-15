#include "func.h"

int main()
{
	int sem_id;
	sem_id=semget(1000,1,0600|IPC_CREAT);
	if(-1==sem_id)
	{
		perror("semget");
		return -1;
	}
	int ret;
	struct semid_ds buf;
	ret=semctl(sem_id,0,IPC_STAT,&buf);
	if(-1==ret)
	{
		perror("semctl");
		return -1;
	}
	printf("cuid=%d,mode=%o,nsems=%ld\n",buf.sem_perm.cuid,buf.sem_perm.mode,buf.sem_nsems);
	buf.sem_perm.mode=0666;
	ret=semctl(sem_id,0,IPC_SET,&buf);
	if(-1==ret)
	{
		perror("semctl");
		return -1;
	}

	return 0;
}
