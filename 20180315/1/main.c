#include "head.h"
#define N 10000000
int main()
{
    int shmid=shmget(1000,4096,0600|IPC_CREAT);
    if(shmid==-1)
    {
        perror("shmget");
        return -1;
    }
    int *p=(int *)shmat(shmid,NULL,0);
    if((int *)-1==p)
    {
        perror("shmat");
        return -1;
    }
    int semid;
    semid=semget(1000,1,0600|IPC_CREAT);
    if(semid==-1)
    {
        perror("semget");
        return -1;
    }
    int ret=semctl(semid,0,SETVAL,1);
    struct sembuf sopp,sopv;
    sopp.sem_num=0;
    sopp.sem_op=-1;
    sopp.sem_flg=SEM_UNDO;
    sopv.sem_num=0;
    sopv.sem_op=1;
    sopv.sem_flg=SEM_UNDO;
    p[0]=0;
    int i;
    if(fork()>0)
    {
        for(i=0;i<N;i++)
        {
            semop(semid,&sopp,1);
            p[0]++;
            semop(semid,&sopv,1);
        }
        wait(NULL);
        printf("%d\n",p[0]);
        ret=shmdt(p);
        if(ret==-1)
        {
            perror("shmdt");
            return -1;
        }
        ret=shmctl(shmid,IPC_RMID,NULL);
        if(ret==-1)
        {
            perror("shmctl");
            return -1;
        }
        ret=semctl(semid,0,IPC_RMID);
        if(ret==-1)
        {
            perror("semctl");
            return -1;
        }
    }
    else{
        for(i=0;i<N;i++)
        {
            semop(semid,&sopp,1);
            p[0]++;
            semop(semid,&sopv,1);

        }
    }
    return 0;
}
