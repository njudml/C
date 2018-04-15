#include   "head.h"
typedef  struct msgbuf
{
    long mtype;
    char mtext[50];
}data;
int main()
{
    int msgid=msgget(1000,0600|IPC_CREAT);
    if(msgid==-1)
    {
        perror("msgget");
        return -1;
    }
    int ret;
    if(fork()>0)
    {
        struct msgbuf t;
        t.mtype=1;
        int i=50;
        memcpy(t.mtext,&i,sizeof(int));
        ret=msgsnd(msgid,&t,4,0);
        if(ret==-1)
        {
            perror("msgsnd");
            return -1;
        }
        wait(NULL);
        ret=msgctl(msgid,IPC_RMID,NULL);
        if(ret==-1)
        {
            perror("msgctl");
            return -1;
        }
    }
    else{
        struct msgbuf t1;
        t1.mtype=1;
        ret=msgrcv(msgid,&t1,sizeof(int),1,0);
        if(ret==-1)
        {
            perror("msgrcv");
            return -1;
        }
        printf("%d\n",*(int *)t1.mtext);
    }
    return 0;
}
