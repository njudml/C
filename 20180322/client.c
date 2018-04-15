#include "head.h"
int main(int argc,char *argv[])
{
    if(argc!=3)
    {
        printf("error args\n");
        return -1;
    }
    struct sockaddr_in serveraddr;
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_addr.s_addr=inet_addr(argv[1]);
    serveraddr.sin_port=htons(atoi(argv[2]));
    int ret;
    int addrlen=sizeof(struct sockaddr);
    int sfd=socket(AF_INET,SOCK_STREAM,0);
    if(sfd==-1)
    {
        perror("socket");
        return -1;
    }
    ret=connect(sfd,(struct sockaddr*)&serveraddr,addrlen);
    if(ret==-1)
    {
        perror("connect");
        close(sfd);
        return -1;
    }
    char buf[512]={0};
    ret=send(sfd,"I am client",11,0);
    if(ret==-1)
    {
        perror("send");
        close(sfd);
        return -1;
    }
    ret=recv(sfd,buf,sizeof(buf),0);
    if(ret==-1)
    {
        perror("recv");
        close(sfd);
        return -1;
    }
    puts(buf);
    close(sfd);
    return 0;
}
