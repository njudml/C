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
	return 0;
}
