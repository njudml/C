#include "head.h"
int main()
{
    int msgid=msgget(1000,IPC_CREAT|0600);
    struct mymsg{
        long mtype;
        char mtext[50];
    };
    struct mymsg msg;
    msg.mtype=1;
    pid_t pid=getpid();
    memcpy(msg.mtext,&pid,sizeof(pid_t));
    msgsnd(msgid,&msg,sizeof(pid_t),0);
    while(1)
    {
        bzero(msg.mtext,sizeof(msg.mtext));
        int ret=msgrcv(msgid,&msg,sizeof(msg.mtext),1,0);
        if(ret==-1)
        {
            perror("msgrcv");
            return -1;
        }
        printf("%s",msg.mtext);
    }
}
