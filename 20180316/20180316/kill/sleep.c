#include "func.h"

void sigfunc(int sig)
{
}

int main()
{
	signal(SIGALRM,sigfunc);
	alarm(3);
	pause();
	return 0;
}
