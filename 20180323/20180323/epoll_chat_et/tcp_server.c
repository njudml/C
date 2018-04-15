#include "func.h"

void set_nonblock(int fd)
{
	int status;
	status=fcntl(fd,F_GETFL);
	status=status|O_NONBLOCK;
	fcntl(fd,F_SETFL,status);
}
int main(int argc,char* argv[])
{
	if(argc!=3)
	{
		printf("./server IP PORT\n");
		return -1;
	}
	int sfd=socket(AF_INET,SOCK_STREAM,0);
	if(-1==sfd)
	{
		perror("socket");
		return -1;
	}
	int reuse=1;
	int ret;
	ret=setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(int));
	if(-1==ret)
	{
		perror("setsockopt");
		return -1;
	}
	struct sockaddr_in ser;
	bzero(&ser,sizeof(ser));
	ser.sin_family=AF_INET;
	ser.sin_port=htons(atoi(argv[2]));//将端口转换为网络字节序
	ser.sin_addr.s_addr=inet_addr(argv[1]);//将点分十进制的ip地址转为32位的网络字节序
	ret=bind(sfd,(struct sockaddr*)&ser,sizeof(ser));
	if(-1==ret)
	{
		perror("bind");
		return -1;
	}
	listen(sfd,10);//激活
	int epfd=epoll_create(1);
	struct epoll_event event,evs[2];
	bzero(&event,sizeof(event));
	event.events=EPOLLIN;//监控读事件
	event.data.fd=0;
	ret=epoll_ctl(epfd,EPOLL_CTL_ADD,0,&event);
	if(-1==ret)
	{
		perror("epoll_ctl");
		return -1;
	}
	struct sockaddr_in cli;
	bzero(&cli,sizeof(cli));
	int addrlen=sizeof(cli);
	int new_fd;
	new_fd=accept(sfd,(struct sockaddr*)&cli,&addrlen);
	if(-1==new_fd)
	{
		perror("accept");
		return -1;
	}
	printf("cli ip=%s,cli port=%d\n",inet_ntoa(cli.sin_addr),ntohs(cli.sin_port));
	bzero(&event,sizeof(event));
	event.events=EPOLLIN|EPOLLET;//监控读事件,采用边沿触发模式
	event.data.fd=new_fd;
	ret=epoll_ctl(epfd,EPOLL_CTL_ADD,new_fd,&event);
	if(-1==ret)
	{
		perror("epoll_ctl");
		return -1;
	}
	set_nonblock(new_fd);//把new_fd设置为非阻塞
	char buf[10]={0};
	int i;
	int nret;
	while(1)
	{
		bzero(evs,sizeof(evs));
		nret=epoll_wait(epfd,evs,2,-1);//nret代表有几个描述符可读
		for(i=0;i<nret;i++)
		{
			if(new_fd==evs[i].data.fd)//new_fd可读
			{
				while(1)
				{
					bzero(buf,sizeof(buf));
					ret=recv(new_fd,buf,sizeof(buf),0);
					if(-1==ret&&errno==EAGAIN)
					{
						break;
					}else if(0==ret)//代表对端断开
					{
						printf("byebye\n");
						goto EXIT;
					}
					printf("%s",buf);
				}
				printf("\n");
			}
			if(evs[i].events==EPOLLIN&&0==evs[i].data.fd)//标准输入可读
			{
				bzero(buf,sizeof(buf));
				ret=read(STDIN_FILENO,buf,sizeof(buf));
				if(ret<=0)
				{
					printf("ret=%d\n",ret);
					goto EXIT;
				}
				send(new_fd,buf,strlen(buf)-1,0);
			}
		}
	}
EXIT:	
	close(new_fd);
	close(sfd);
	return 0;
}
