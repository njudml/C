#include "func.h"
//客户端断开后，服务器不断开，客户端再次连接，可以连上

void epoll_add(int epfd,int fd)
{
	struct epoll_event event;
	bzero(&event,sizeof(event));
	event.events=EPOLLIN;//监控读事件
	event.data.fd=fd;
	int ret=epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&event);
	if(-1==ret)
	{
		perror("epoll_ctl");
		return;
	}
}
void epoll_del(int epfd,int fd)
{
	struct epoll_event event;
	bzero(&event,sizeof(event));
	event.events=EPOLLIN;//监控读事件
	event.data.fd=fd;
	int ret=epoll_ctl(epfd,EPOLL_CTL_DEL,fd,&event);
	if(-1==ret)
	{
		perror("epoll_ctl");
		return;
	}
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
	struct epoll_event event,evs[3];
	int new_fd;
	struct sockaddr_in cli;
	int addrlen=sizeof(cli);
	epoll_add(epfd,0);//监控标准输入
	epoll_add(epfd,sfd);//监控socket描述符
	char buf[128]={0};
	int i;
	int nret;
	while(1)
	{
		bzero(evs,sizeof(evs));
		nret=epoll_wait(epfd,evs,3,-1);//nret代表有几个描述符可读
		for(i=0;i<nret;i++)
		{
			if(sfd==evs[i].data.fd)
			{
				bzero(&cli,sizeof(cli));
				new_fd=accept(sfd,(struct sockaddr*)&cli,&addrlen);
				if(-1==new_fd)
				{
					perror("accept");
					return -1;
				}
				printf("cli ip=%s,cli port=%d\n",inet_ntoa(cli.sin_addr),ntohs(cli.sin_port));
				epoll_add(epfd,new_fd);//注册new_fd
			}
			if(new_fd==evs[i].data.fd)//new_fd可读
			{
				bzero(buf,sizeof(buf));
				ret=recv(new_fd,buf,sizeof(buf),0);
				if(-1==ret)
				{
					perror("recv");
					return -1;
				}else if(0==ret)//代表对端断开
				{
					printf("byebye\n");
					epoll_del(epfd,new_fd);
					close(new_fd);
					break;
				}
				printf("%s\n",buf);
			}
			if(evs[i].events==EPOLLIN&&0==evs[i].data.fd)//标准输入可读
			{
				bzero(buf,sizeof(buf));
				ret=read(STDIN_FILENO,buf,sizeof(buf));
				if(0==ret)
				{
					printf("byebye\n");
					epoll_del(epfd,new_fd);
					close(new_fd);
					break;
				}
				send(new_fd,buf,strlen(buf)-1,0);
			}
		}
	}
	close(sfd);
	return 0;
}
