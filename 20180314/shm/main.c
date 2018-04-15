#include "head.h"
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
        char *p=(char *)shmat(ret,NULL,0);
        memset(p,0,4096);
        strncpy(p,"How are you",4096);
        wait(NULL);

    }else{
        sleep(5);
        char *p1=(char *)shmat(ret,NULL,0);
        puts(p1);
    }

    return 0;
}
