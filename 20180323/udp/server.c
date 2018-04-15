#include "head.h"
int main(int argc,char *argv[])
{
    if(argc!=3)
    {
        printf("error args\n");
        return -1;
    }
    int sfd=socket(AF_INET,SOCK_DGRAM,0);
    if(sfd==-1)
    {
        perror("socket");
        return -1;
    }
    struct sockaddr_in ser;
    ser.sin_family=AF_INET;
    ser.sin_addr.s_addr=inet_addr(argv[1]);
    ser.sin_port=htons(atoi(argv[2]));
    int len=sizeof(struct sockaddr);
    int ret=bind(sfd,(struct sockaddr*)&ser,len);
    if(ret==-1)
    {
        perror("bind");
        close(sfd);
        return -1;
    }
    struct sockaddr_in cli;
    char buf[512]={0};
    bzero(&cli,sizeof(cli));
    ret=recvfrom(sfd,buf,sizeof(buf),0,(struct sockaddr*)&cli,&len);
    if(ret==-1)
    {
        printf("%d  ",errno);
        perror("recvform");
        return -1;
    }
    printf("%s  received from %s %d\n",buf,inet_ntoa(cli.sin_addr),ntohs(cli.sin_port));
    sendto(sfd,"world",5,0,(struct sockaddr*)&cli,len);
    close(sfd);
}
