#include "func.h"
void sigfunc(int sig)
{
	printf("%d is coming\n",sig);
}

int main()
{
	signal(SIGINT,sigfunc);
	sigset_t c_mask;
	sigemptyset(&c_mask);
	sigaddset(&c_mask,SIGINT);
	int ret;
	ret=sigprocmask(SIG_BLOCK,&c_mask,NULL);
	if(-1==ret)
	{
		perror("sigprocmask");
		return -1;
	}
	printf("before sleep\n");//关键代码
	sleep(5);
	sigset_t set;
	sigemptyset(&set);
	sigpending(&set);
	if(sigismember(&set,SIGINT))
	{
		printf("SIGINT is pending\n");
	}else{
		printf("SIGINT is not pending\n");
	}
	printf("after sleep\n");
	sigprocmask(SIG_UNBLOCK,&c_mask,NULL);
	return 0;
}
