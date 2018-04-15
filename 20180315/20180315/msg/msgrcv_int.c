#include "func.h"

typedef struct msgbuf{
		long mtype;
		char mtext[100];
}data;
int main()
{
	int msgid;
	msgid=msgget(1000,IPC_CREAT|0600);
	if(-1==msgid)
	{
		perror("msgget");
		return -1;
	}
	data t;
	bzero(&t,sizeof(t));
	t.mtype=1;
	int ret=msgrcv(msgid,&t,sizeof(t.mtext),1,0);
	if(-1==ret)
	{
		perror("msgsnd");
		return -1;
	}
	printf("%d\n",*(int*)t.mtext);
	return 0;
}
