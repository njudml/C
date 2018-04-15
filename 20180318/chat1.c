#include "head.h"
pid_t pid3,pid2;
int shmid,semid;
void handle(int sig)
{
    unlink("1");
    unlink("2");
    semctl(semid,0,IPC_RMID);
    shmctl(shmid,IPC_RMID,NULL);
    kill(pid3,SIGINT);
    kill(pid2,SIGINT);
    kill(0,SIGKILL);
}

int main()
{
    mkfifo("1",0666);
    mkfifo("2",0666);
    int fdr,fdw;
    fdr=open("1",O_RDONLY);
    if(fdr==-1)
    {
        perror("open1");
        return -1;
    }
    fdw=open("2",O_WRONLY);
    if(fdw==-1)
    {
        perror("open2");
        return -1;
    }
    shmid=shmget(1000,4096,0600|IPC_CREAT);
    if(shmid==-1)
    {
        perror("shmget");
        return -1;
    }
    semid=semget(1000,2,0600|IPC_CREAT);
    if(semid==-1)
    {
        perror("semget");
        return -1;
    }
    unsigned short a[2]={1,0};
    semctl(semid,0,SETALL,a);
    struct sembuf sbp[2],sbv[2];
    sbp[0].sem_num=0;
    sbp[0].sem_op=-1;
    sbp[0].sem_flg=SEM_UNDO;
    sbv[0].sem_num=1;
    sbv[0].sem_op=1;
    sbv[0].sem_flg=SEM_UNDO;
    sbp[1].sem_num=1;
    sbp[1].sem_op=-1;
    sbp[1].sem_flg=SEM_UNDO;
    sbv[1].sem_num=0;
    sbv[1].sem_op=1;
    sbv[1].sem_flg=SEM_UNDO;
        
    fd_set fdset;
    char buf[100]={0};
    int ret;
    int *p1=(int *)shmat(shmid,NULL,0);
    semop(semid,&sbp[1],1);
    pid3=*p1;
    semop(semid,&sbv[1],1);
    signal(SIGINT,handle);
    char buf2[100]={0};
    read(fdr,buf2,sizeof(pid_t));
    pid2=*(int *)buf2;
    pid_t pid=getpid();
    memcpy(buf2,&pid,sizeof(pid));
    write(fdw,buf2,sizeof(pid_t));

    while(1)
    {
        FD_ZERO(&fdset);
        FD_SET(0,&fdset);
        FD_SET(fdr,&fdset);
        select(fdr+1,&fdset,NULL,NULL,NULL);
        if(FD_ISSET(fdr,&fdset))
        {
            bzero(buf,sizeof(buf));
            ret=read(fdr,buf,sizeof(buf));
            if(ret==0)
                break;
            char *p=(char *)shmat(shmid,NULL,0);
            memset(p,0,4096);
            semop(semid,&sbp[0],1);
            strcpy(p,"B: ");
            strcat(p,buf);
            semop(semid,&sbv[0],1);

        }
        if(FD_ISSET(0,&fdset))
        {
            bzero(buf,sizeof(buf));
            ret=read(0,buf,sizeof(buf)-1);
            if(ret==0)
                break;
            write(fdw,buf,strlen(buf));
            char *p=(char *)shmat(shmid,NULL,0);
            memset(p,0,4096);
            semop(semid,&sbp[0],1);
            strcpy(p,"A: ");
            strcat(p,buf);
            semop(semid,&sbv[0],1);

        }
    }
    kill(pid3,SIGINT);
}
