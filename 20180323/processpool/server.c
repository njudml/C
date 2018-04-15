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
    if(argc!=4)
    {
        printf("error args\n");
        return -1;
    }
    int num=atoi(argv[3]);
    pdata p=(pdata)calloc(num,sizeof(data));
    makechild(p,num);
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
    struct epoll_event *evs=(struct epoll_event*)calloc(num+1,sizeof(struct epoll_event));
    epoll_add(esfd,sfd);
    struct sockaddr_in cli;
    
    char buf[512]={0};
    int nret;
    int i,j;
    for(i=0;i<num;i++)
    {
        epoll_add(esfd,p[i].fd);
    }
    int new_fd;
    char flag;
    while(1)
    {
        bzero(evs,(num+1)*sizeof(struct epoll_event));
        nret=epoll_wait(esfd,evs,num+1,-1);
        for(i=0;i<nret;i++)
        {
            if(sfd==evs[i].data.fd)
            {
                new_fd=accept(sfd,NULL,NULL);
                for(j=0;j<num;j++)
                {
                    if(p[j].busy==0)
                        break;
                }
                send_fd(p[i].fd,new_fd);
                close(new_fd);
                p[j].busy=1;
                printf("%d is busy\n",p[j].pid);
            }
            for(j=0;j<num;j++)
            {
                if(p[j].fd==evs[i].data.fd)
                {
                    read(p[j].fd,&flag,sizeof(char));
                    p[j].busy=0;
                    printf("%d is not busy\n",p[j].pid);
                }
            }
        }
    }
    close(sfd);


}
