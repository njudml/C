#include "head.h"
int main(int argc,char *argv[])
{
    if(argc!=3)
    {
        printf("error args\n");
        return -1;
    };
    int sfd=socket(AF_INET,SOCK_DGRAM,0);
    if(sfd==-1)
    {
        perror("socket");
        return -1;
    }
    struct sockaddr_in cli;
    cli.sin_family=AF_INET;
    cli.sin_addr.s_addr=inet_addr(argv[1]);
    cli.sin_port=htons(atoi(argv[2]));
    int len=sizeof(struct sockaddr);
    char buf[512]={0};
    int ret;
    sendto(sfd,"hello",5,0,(struct sockaddr*)&cli,len);
    ret=recvfrom(sfd,buf,sizeof(buf),0,(struct sockaddr*)&cli,&len);
    if(ret==-1)
    {
        perror("recvform");
        return -1;
    }
    printf("%s  received from %s %d\n",buf,inet_ntoa(cli.sin_addr),ntohs(cli.sin_port));
    close(sfd);
}
