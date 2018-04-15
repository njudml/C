#include "head.h"
int main(int argc,char *argv[])
{
    if(argc!=3)
    {
        printf("error args\n");
        return -1;
    }
    int sfd=socket(AF_INET,SOCK_STREAM,0);
    int ret;
    struct sockaddr_in cli;
    cli.sin_family=AF_INET;
    cli.sin_addr.s_addr=inet_addr(argv[1]);
    cli.sin_port=htons(atoi(argv[2]));
    int len=sizeof(struct sockaddr);
    connect(sfd,(struct sockaddr*)&cli,len);
    int epfd=epoll_create(1);
    struct epoll_event event,evs[2];
    event.events=EPOLLIN;
    event.data.fd=0;
    epoll_ctl(epfd,EPOLL_CTL_ADD,0,&event);
    event.data.fd=sfd;
    epoll_ctl(epfd,EPOLL_CTL_ADD,sfd,&event);
    int i;
    int nret;
    char buf[512]={0};
    while(1)
    {
        bzero(evs,sizeof(evs));
        nret=epoll_wait(epfd,evs,2,-1);
        for(i=0;i<nret;i++)
        {
            if(0==evs[i].data.fd)
            {
                bzero(buf,sizeof(buf));
                ret=read(0,buf,sizeof(buf));
                if(ret==0)
                {
                    goto EXIT;   
                }
                send(sfd,buf,strlen(buf)-1,0);
            }
            if(sfd==evs[i].data.fd)
            {
                bzero(buf,sizeof(buf));
                ret=recv(sfd,buf,sizeof(buf),0);
                if(ret==0)
                    goto EXIT;
                puts(buf);
            }
        }
    }
EXIT:
    close(sfd);
    return 0;

}
