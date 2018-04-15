#include "head.h"
void handle(int fd) 
{
    int i;
    int sfd;
    char unbusy='1';
    while(1)
    {
        recv_fd(fd,&sfd);
        for(i=0;i<10;i++)
        {
            send(sfd,"ok",2,0);
            sleep(5);
        }
        write(fd,&unbusy,sizeof(char));
    }
}
void makechild(pdata p,int n)
{
    int i;
    int fds[2];
    pid_t pid;
    for(i=0;i<n;i++)
    {
        socketpair(AF_LOCAL,SOCK_STREAM,0,fds);
        pid=fork();
        if(!pid)
        {
            close(fds[0]);
            handle(fds[1]);
        }
        close(fds[1]);
        p[i].pid=pid;
        p[i].fd=fds[0];

    }
}
