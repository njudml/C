#include "func.h"

int main(int argc,char* argv[])
{
	int shmid;
	shmid=shmget(1000,1024,0600|IPC_CREAT);
	if(-1==shmid)
	{
		perror("shmget");
		return -1;
	}
	struct shmid_ds buf;
	int ret;
	ret=shmctl(shmid,IPC_STAT,&buf);
	if(-1==ret)
	{
		perror("shmctl");
		return -1;
	}
	printf("cuid=%d,mode=%o,size=%ld,nattach=%ld\n",buf.shm_perm.cuid,buf.shm_perm.mode,buf.shm_segsz,buf.shm_nattch);
	buf.shm_perm.mode=0666;
	buf.shm_segsz=1024;
	ret=shmctl(shmid,IPC_SET,&buf);
	if(-1==ret)
	{
		perror("shmctl");
		return -1;
	}
	return 0;
}
