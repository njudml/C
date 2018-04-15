#include "head.h"
void epoll_add(int epfd,int fd)
{
    struct epoll_event event;
    event.events=EPOLLIN;
    event.data.fd=fd;
    epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&event);
}
int main(int argc,char *argv[])
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
    struct epoll_event *evs=(struct epoll_event*)calloc(num+1,sizeof(struct epoll_event));
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
        bzero(evs,sizeof(struct epoll_event)*(num+1));
        nret=epoll_wait(epfd,evs,num+1,-1);
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
                send_fd(p[j].fd,new_fd);
                close(new_fd);
                p[j].busy=1;
                printf("process %d is busy\n",p[j].pid);
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
