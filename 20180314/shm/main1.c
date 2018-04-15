#include "head.h"
#define N 10000000
int main(int argc,char *argv[])
{
    if(argc!=2)
    { 
        perror("error args\n");
        return -1;
    }
    key_t key;
    key=ftok(argv[1],1);
    int ret=shmget(key,4096,0600|IPC_CREAT);
    if(ret==-1) 
    {
        perror("shmget");
        return -1;
    }
    if(fork()>0)
    {
        int * p=(int *)shmat(ret,NULL,0);
        p[0]=0;
        for(int i=0;i<N;i++)
            p[0]++;
        wait(NULL);
        printf("%d\n",p[0]);
    }else{
        int *p1=(int *)shmat(ret,NULL,0);
        for(int i=0;i<N;i++)
            p1[0]++;
    }

    return 0;
}
