#include "head.h"
void epoll_add(int epfd,int fd)
{
    struct epoll_event event;
    event.events=EPOLLIN;
    event.data.fd=fd;
    epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&event);
}
void epoll_del(int epfd,int fd)
{
    struct epoll_event event;
    event.events=EPOLLIN;
    event.data.fd=fd;
    epoll_ctl(epfd,EPOLL_CTL_DEL,fd,&event);
}
int fds[2];
void sigfunc(int sig)
{
    char flag;
    write(fds[1],&flag,sizeof(char));
}
int main(int argc,char *argv[])
{
    if(argc!=4)
    {
        printf("error args\n");
        return -1;
    }
    pipe(fds);
    signal(SIGUSR1,sigfunc);
    int num=atoi(argv[3]);
    pdata p=(pdata)calloc(num,sizeof(data));
    makechild(p,num);
    int sfd=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in ser;
    int reuse=1;
    setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(int));
    ser.sin_family=AF_INET;
    ser.sin_addr.s_addr=inet_addr(argv[1]);
    ser.sin_port=htons(atoi(argv[2]));
    int addrlen=sizeof(struct sockaddr);
    bind(sfd,(struct sockaddr*)&ser,addrlen);
    listen(sfd,10);
    int epfd=epoll_create(1);
    epoll_add(epfd,sfd);
    epoll_add(epfd,fds[0]);
    struct epoll_event *evs=(struct epoll_event*)calloc(num+2,sizeof(struct epoll_event));
    int i,j;
    for(i=0;i<num;i++)
    {
        epoll_add(epfd,p[i].fd);
    }
    int nret;
    int new_fd;
    char flag;
    while(1)
    {
        bzero(evs,sizeof(struct epoll_event)*(num+2));
        nret=epoll_wait(epfd,evs,num+2,-1);
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
                send_fd(p[j].fd,new_fd,0);
                close(new_fd);
                p[j].busy=1;
                printf("process %d is busy\n",p[j].pid);
            }
            if(fds[0]==evs[i].data.fd)
            {
                epoll_del(epfd,sfd);
                for(j=0;j<num;j++)
                {
                    send_fd(p[j].fd,0,1);
                }
                for(j=0;j<num;j++)
                {
                    wait(NULL);
                }
            }
            for(j=0;j<num;j++)
            {
                if(p[j].fd==evs[i].data.fd)
                {
                    read(p[j].fd,&flag,sizeof(char));
                    printf("process %d is not busy\n",p[j].pid);
                    p[j].busy=0;
                }
            }
            
        }
    }


}
