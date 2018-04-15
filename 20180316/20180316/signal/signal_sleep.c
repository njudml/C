#include "func.h"
void sigfunc(int sig)
{
	printf("before sleep %d is coming\n",sig);
	sleep(3);
	printf("after sleep %d is coming\n",sig);
}

int main()
{
	if(signal(SIGINT,sigfunc)==SIG_ERR)
	{
		perror("signal");
		return -1;
	}
	signal(SIGQUIT,sigfunc);
	while(1);
}
