#include "func.h"

void sigfunc(int sig,siginfo_t *p,void* p1)
{
	printf("before %d is coming\n",sig);
	sleep(3);
	sigset_t set;
	sigemptyset(&set);
	sigpending(&set);
	if(sigismember(&set,SIGQUIT))
	{
		printf("SIGQUIT is pending\n");
	}else{
		printf("SIGQUIT is not pending\n");
	}
	printf("after %d is coming\n",sig);
}
//在2号的信号处理流程过程中，3号信号到来，不执行3号信号，等2号信号的信号处理流程结束，才会响应3号信号
int main()
{
	struct sigaction act;
	memset(&act,0,sizeof(act));
	act.sa_sigaction=sigfunc;
	act.sa_flags=SA_SIGINFO;
	sigemptyset(&act.sa_mask);//清空集合
	sigaddset(&act.sa_mask,SIGQUIT);
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
