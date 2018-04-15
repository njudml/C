#include "func.h"

void sigfunc(int sig,siginfo_t *p,void* p1)
{
	printf("%d is coming,val=%p\n",sig,(char*)p->si_ptr);
}

int main()
{
	struct sigaction act;
	memset(&act,0,sizeof(act));
	act.sa_sigaction=sigfunc;
	act.sa_flags=SA_SIGINFO|SA_RESETHAND;
	int ret;
	ret=sigaction(SIGINT,&act,NULL);
	if(-1==ret)
	{
		perror("sigaction");
		return -1;
	}
	char data[10]="hello";
	union sigval a;
	a.sival_ptr=data;
	//sigqueue(getpid(),SIGINT,a);
	kill(0,SIGINT);
	while(1);
	return 0;
}
