#include "func.h"

void sigfunc(int sig,siginfo_t *p,void* p1)
{
	printf("%d is coming\n",sig);
}

int main()
{
	struct sigaction act;
	memset(&act,0,sizeof(act));
	act.sa_sigaction=sigfunc;
	act.sa_flags=SA_SIGINFO;
	int ret;
	ret=sigaction(SIGINT,&act,NULL);
	if(-1==ret)
	{
		perror("sigaction");
		return -1;
	}
	if(!fork())
	{
		signal(SIGINT,SIG_IGN);
		while(1);
	}else{
		while(1);
	}
	return 0;
}
