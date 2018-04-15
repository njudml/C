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
	char buf[128]={0};
	fd_set rdset;
	struct timeval t;
	bzero(&t,sizeof(t));
	while(1)
	{
		FD_ZERO(&rdset);
		FD_SET(STDIN_FILENO,&rdset);
		FD_SET(new_fd,&rdset);
		t.tv_sec=5;
		ret=select(new_fd+1,&rdset,NULL,NULL,&t);//timeout是传入传出参数
		if(ret>0)
		{
			if(FD_ISSET(new_fd,&rdset))//new_fd可读
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
					printf("ret=%d\n",ret);
					break;
				}
				send(new_fd,buf,strlen(buf)-1,0);
			}
		}
		printf("select ret=%d\n",ret);
	}		
	close(new_fd);
	close(sfd);
	return 0;
}
