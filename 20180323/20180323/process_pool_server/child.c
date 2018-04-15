#include "func.h"
//创建子进程
void make_child(pData p,int n)
{
	int i;
	pid_t pid;
	int fds[2];
	for(i=0;i<n;i++)
	{
		socketpair(AF_LOCAL,SOCK_STREAM,0,fds);
		pid=fork();
		if(!pid)
		{
			close(fds[0]);
			child_handle(fds[1]);
		}
		close(fds[1]);
		p[i].pid=pid;
		p[i].fd=fds[0];
		printf("pid=%d,fds[0]=%d\n",pid,fds[0]);
	}
}

void child_handle(int fd)
{
	int sfd;
	char unbusy='o';
	while(1)
	{
		recv_fd(fd,&sfd);//接收任务
		printf("I will send file to customer %d\n",sfd);
		send(sfd,"ok",2,0);
		write(fd,&unbusy,sizeof(char));//告诉父进程子进程非忙碌
	}
}
