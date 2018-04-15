#include "func.h"
void set_noblock(int fd)
{
	int status;
	status=fcntl(fd,F_GETFL);
	status=status|O_NONBLOCK;
	fcntl(fd,F_SETFL,status);
}


int main()
{
	char buf[128]={0};
	int ret;
	sleep(5);
	set_noblock(0);
	ret=read(0,buf,sizeof(buf));
	printf("ret=%d,buf=%s,errno=%d\n",ret,buf,errno);
	return 0;
}
