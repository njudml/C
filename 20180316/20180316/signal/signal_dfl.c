#include "func.h"

int main()
{
	signal(SIGINT,SIG_IGN);//信号的忽略
	sleep(6);
	signal(SIGINT,SIG_DFL);
	while(1);
}
