#include "func.h"

void sigfunc(int sig,siginfo_t *p,void* p1)
{
	printf("before %d is coming\n",sig);
	sleep(3);
	printf("after %d is coming\n",sig);
}

int main()
{
	struct sigaction act;
	memset(&act,0,sizeof(act));
	act.sa_sigaction=sigfunc;
	act.sa_flags=SA_SIGINFO|SA_NODEFER;
	int ret;
	ret=sigaction(SIGINT,&act,NULL);
	if(-1==ret)
	{
		perror("sigaction");
		return -1;
	}
	sigaction(SIGQUIT,&act,NULL);
	while(1);
	return 0;
}
