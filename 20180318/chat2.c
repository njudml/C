#include "head.h"
struct mymsg{
    long mtype;
    char mtext[50];
};
pid_t pid4,pid1;
int msgid;
void handle(int sig)
{
    msgctl(msgid,IPC_RMID,NULL);
    kill(pid4,SIGINT);
    kill(pid1,SIGINT);
    kill(0,SIGKILL);
}
int main()
{
    int fdr,fdw;
    fdw=open("1",O_WRONLY);
    if(fdw==-1)
    {
        perror("open1");
        return -1;
    }
    fdr=open("2",O_RDONLY);
    if(fdr==-1)
    {
        perror("open2");
        return -1;
    }
    msgid=msgget(1000,IPC_CREAT|0600);
    if(msgid==-1)
    {
    perror("msgget");
    }
    struct mymsg msg;
    msg.mtype=1;
    fd_set fdset;
    int ret;
    char buf[100]={0};
    msgrcv(msgid,&msg,sizeof(pid_t),1,0);
    pid4=*(int *)msg.mtext;
    pid_t pid=getpid();
    char buf2[100]={0};
    memcpy(buf2,&pid,sizeof(pid));
    write(fdw,buf2,sizeof(pid_t));
    read(fdr,buf2,sizeof(pid_t));
    pid1=*(int *)buf2;

    signal(SIGINT,handle);
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
            strcpy(msg.mtext,"A :");
            strcat(msg.mtext,buf);
            ret=msgsnd(msgid,&msg,strlen(msg.mtext),0);
            if(ret==-1)
            {
                perror("msgsnd");
            }
        }
        else if(FD_ISSET(0,&fdset))
        {
            bzero(buf,sizeof(buf));
            ret=read(0,buf,sizeof(buf)-1);
            if(ret==0)
                break;
            write(fdw,buf,strlen(buf));
            strcpy(msg.mtext,"B :");
            strcat(msg.mtext,buf);
            ret=msgsnd(msgid,&msg,strlen(msg.mtext),0);
            if(ret==-1)
            {
                perror("msgsnd");
            }
        }
    }
    kill(pid4,SIGINT);
    msgctl(msgid,IPC_RMID,NULL);
    return 0;
}
