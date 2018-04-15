#include "func.h"

int main(int argc,char* argv[])
{
	if(argc!=3)
	{
		printf("./server IP PORT\n");
		return -1;
	}
	int sfd=socket(AF_INET,SOCK_DGRAM,0);
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
	ret=bind(sfd,(struct sockaddr*)&ser,sizeof(ser));
	if(-1==ret)
	{
		perror("bind");
		return -1;
	}
	//服务器先收
	struct sockaddr_in cli;
	int len=sizeof(struct sockaddr);
	char buf[128]={0};
	ret=recvfrom(sfd,buf,sizeof(buf),0,(struct sockaddr*)&cli,&len);
	if(-1==ret)
	{
		perror("recvfrom");
		return -1;
	}
	puts(buf);
	sendto(sfd,"world",5,0,(struct sockaddr*)&cli,sizeof(struct sockaddr));
	close(sfd);
	return 0;
}
