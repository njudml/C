#include "head.h"
int main(int argc,char *argv[])
{
    if(argc!=3)
    {
        printf("error args\n");
        return -1;
    }
    int sfd=socket(AF_INET,SOCK_STREAM,0);
    if(sfd==-1)
    {
        perror("socket");
        return -1;
    }
    struct sockaddr_in myaddr;
    memset(&myaddr,0,sizeof(myaddr));
    int addrlen=sizeof(struct sockaddr);
    myaddr.sin_family=AF_INET;
    myaddr.sin_addr.s_addr=inet_addr(argv[1]);
    myaddr.sin_port=htons(atoi(argv[2]));
    int ret=bind(sfd,(struct sockaddr *)&myaddr,addrlen);
    if(ret==-1)
    {
        perror("bind");
        close(sfd);
        return -1;
    }
    if(listen(sfd,10)==-1)
    {
        perror("listen");
        close(sfd);
        return -1;
    }
    struct sockaddr_in clientaddr;
    bzero(&clientaddr,addrlen);
    int new_sfd=accept(sfd,(struct sockaddr*)&clientaddr,&addrlen);
    if(new_sfd==-1)
    {
        perror("accept");
        close(sfd);
        return -1;
    }
    printf("%s %d connected\n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));
    char buf[512]={0};
    ret=recv(new_sfd,buf,sizeof(buf),0);
    if(ret==-1)
    {
        perror("recv");
        close(new_sfd);
        close(sfd);
        return -1;
    }
    puts(buf);
    ret=send(new_sfd,"I am server",11,0);
    if(ret==-1)
    {
        perror("send");
        close(new_sfd);
        close(sfd);
        return -1;
    }
    close(new_sfd);
    close(sfd);
    return 0; 
}
