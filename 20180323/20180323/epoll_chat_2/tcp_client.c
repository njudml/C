#include "func.h"

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
	struct sockaddr_in ser;
	bzero(&ser,sizeof(ser));
	ser.sin_family=AF_INET;
	ser.sin_port=htons(atoi(argv[2]));//将端口转换为网络字节序
	ser.sin_addr.s_addr=inet_addr(argv[1]);//将点分十进制的ip地址转为32位的网络字节序
	int ret;
	ret=connect(sfd,(struct sockaddr*)&ser,sizeof(ser));
	if(-1==ret)
	{
		perror("connect");
		return -1;
	}
	printf("I am client\n");
	char buf[128]={0};
	fd_set rdset;
	while(1)
	{
		FD_ZERO(&rdset);
		FD_SET(STDIN_FILENO,&rdset);
		FD_SET(sfd,&rdset);
		ret=select(sfd+1,&rdset,NULL,NULL,NULL);
		if(ret>0)
		{
			if(FD_ISSET(sfd,&rdset))//sfd可读
			{
				bzero(buf,sizeof(buf));
				ret=recv(sfd,buf,sizeof(buf),0);
				if(-1==ret)
				{
					perror("recv");
					return -1;
				}else if(0==ret)
				{
					printf("byebye\n");
					break;
				}
				printf("%s\n",buf);
			}
			if(FD_ISSET(0,&rdset))//标准输入可读
			{
				bzero(buf,sizeof(buf));
				ret=read(STDIN_FILENO,buf,sizeof(buf));
				if(ret<=0)
				{
					printf("read ret=%d\n",ret);
					break;
				}
				send(sfd,buf,strlen(buf)-1,0);
			}
		}
	}		
	close(sfd);
	return 0;
}

