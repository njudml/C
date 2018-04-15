#include "head.h"
int main()
{
    int shmid,semid;
    shmid=shmget(1000,4096,0600);
    char *p=(char *)shmat(shmid,NULL,0);
    semid=semget(1000,2,IPC_CREAT);
    struct sembuf sbp[2],sbv[2];
    sbp[0].sem_num=1;
    sbp[0].sem_op=-1;
    sbp[0].sem_flg=SEM_UNDO;
    sbv[0].sem_num=0;
    sbv[0].sem_op=1;
    sbv[0].sem_flg=SEM_UNDO;
    sbp[1].sem_num=0;
    sbp[1].sem_op=-1;
    sbp[1].sem_flg=SEM_UNDO;
    sbv[1].sem_num=1;
    sbv[1].sem_op=1;
    sbv[1].sem_flg=SEM_UNDO;
    pid_t pid=getpid();
    printf("pid3=%d\n",pid);
    int *p1=(int *)shmat(shmid,NULL,0);
    semop(semid,&sbp[1],1);
    *p1=pid;
    semop(semid,&sbv[1],1); 
    printf("%d\n",*p1);
    while(1)
    {
        semop(semid,&sbp[0],1);
        printf("%s",p); 
        memset(p,0,4096);
        semop(semid,&sbv[0],1);
    }
}
