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
	signal(SIGINT,SIG_IGN);
	printf("before sleep\n");
	sleep(10);
	printf("after sleep\n");
	signal(SIGINT,SIG_DFL);
	return 0;
}
