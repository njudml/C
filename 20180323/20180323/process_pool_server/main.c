#include "func.h"
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

int main(int argc,char* argv[])
{
	if(argc!=4)
	{
		printf("./server IP PORT PROCESS_NUM\n");
		return -1;
	}
	int num=atoi(argv[3]);
	pData p=(pData)calloc(num,sizeof(Data));
	make_child(p,num);
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
	int epfd=epoll_create(1);
	epoll_add(epfd,sfd);
	int i,j;
	for(i=0;i<num;i++)
	{
		epoll_add(epfd,p[i].fd);
	}
	listen(sfd,10);//激活
	struct epoll_event *evs=(struct epoll_event*)calloc(num+1,sizeof(struct epoll_event));
	int new_fd;
	char flag;
	while(1)
	{
		bzero(evs,sizeof(struct epoll_event)*(num+1));
		ret=epoll_wait(epfd,evs,num+1,-1);
		for(i=0;i<ret;i++)
		{
			if(sfd==evs[i].data.fd)
			{
				new_fd=accept(sfd,NULL,NULL);
				//找到非忙碌子进程，把new_fd传递给它
				for(j=0;j<num;j++)
				{
					if(0==p[j].busy)
					{
						break;
					}
				}
				send_fd(p[j].fd,new_fd);
				close(new_fd);	
				p[j].busy=1;//把子进程置为忙碌状态
				printf("p[j].pid=%d is busy\n",p[j].pid);
			}
			for(j=0;j<num;j++)//发现对应子进程的描述符可读，设置对应的子进程为非忙碌
			{
				if(p[j].fd==evs[i].data.fd)
				{
					read(p[j].fd,&flag,sizeof(char));
					printf("p[j].pid=%d is not busy\n",p[j].pid);
					p[j].busy=0;
				}
			}
		}
	}
}

