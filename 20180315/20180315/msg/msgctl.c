#include "func.h"

int main()
{
	int msgid;
	msgid=msgget(1000,IPC_CREAT|0600);
	if(-1==msgid)
	{
		perror("msgget");
		return -1;
	}
	int ret;
	ret=msgctl(msgid,IPC_RMID,NULL);
	if(-1==ret)
	{
		perror("msgctl");
		return -1;
	}
	return 0;
}
