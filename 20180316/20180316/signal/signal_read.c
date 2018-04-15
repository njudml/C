#include "func.h"
void sigfunc(int sig)
{
	printf("%d is coming\n",sig);
}

int main()
{
	if(signal(SIGINT,sigfunc)==SIG_ERR)
	{
		perror("signal");
		return -1;
	}
	char buf[128]={0};
	read(0,buf,sizeof(buf));
}
