#include "head.h"
void epoll_add(int esfd,int fd)
{
    struct epoll_event event;
    bzero(&event,sizeof(event));
    event.events=EPOLLIN;
    event.data.fd=fd;
    epoll_ctl(esfd,EPOLL_CTL_ADD,fd,&event);
}
void epoll_del(int esfd,int fd)
{
    struct epoll_event event;
    bzero(&event,sizeof(event));
    event.events=EPOLLIN;
    event.data.fd=fd;
    epoll_ctl(esfd,EPOLL_CTL_ADD,fd,&event);
}
int main(int argc,char*argv[])
{
    if(argc!=3)
    {
        printf("error args\n");
        return -1;
    }
    int sfd=socket(AF_INET,SOCK_STREAM,0);
    int reuse=1;
    int ret;
    setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(int));
    struct sockaddr_in ser;
    bzero(&ser,sizeof(ser));
    ser.sin_family=AF_INET;
    ser.sin_addr.s_addr=inet_addr(argv[1]);
    ser.sin_port=htons(atoi(argv[2]));
    int len=sizeof(struct sockaddr);
    bind(sfd,(struct sockaddr*)&ser,len);
    listen(sfd,10);
    int esfd=epoll_create(1);
    struct epoll_event event,ev[3];
    epoll_add(esfd,0);
    epoll_add(esfd,sfd);
    struct sockaddr_in cli;
    
    char buf[512]={0};
    int nret;
    int i;
    int new_fd;
    while(1)
    {
        bzero(ev,sizeof(ev));
        nret=epoll_wait(esfd,ev,3,-1);
        for(i=0;i<nret;i++)
        {
            if(sfd==ev[i].data.fd)
            {
                new_fd=accept(sfd,(struct sockaddr*)&cli,&len);
                printf("%s %d connected\n",inet_ntoa(cli.sin_addr),ntohs(cli.sin_port));
                epoll_add(esfd,new_fd);
            }
            if(new_fd==ev[i].data.fd)
            {
                bzero(buf,sizeof(buf));
                ret=recv(new_fd,buf,sizeof(buf),0);
                if(ret==0)
                {
                    printf("byebye\n");
                    close(new_fd);
                    epoll_del(esfd,new_fd);
                    break;
                }
                puts(buf);
            }
            if(0==ev[i].data.fd)
            {
                bzero(buf,sizeof(buf));
                ret=read(0,buf,sizeof(buf));
                if(ret==0)
                {
                    close(new_fd);
                    epoll_del(esfd,new_fd);
                    break;
                }
                send(new_fd,buf,strlen(buf)-1,0);

            }
        }
    }
    close(sfd);


}
