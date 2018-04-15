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
	int i=100;
	t.mtype=1;
	memcpy(t.mtext,&i,sizeof(int));
	int ret=msgsnd(msgid,&t,4,0);
	if(-1==ret)
	{
		perror("msgsnd");
		return -1;
	}
	return 0;
}
