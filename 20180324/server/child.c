#include "head.h"
void handle(int fd)
{
    int sfd;
    char unbusy='0';
    while(1)
    {
        recv_fd(fd,&sfd);
        send_file(sfd); 
        write(fd,&unbusy,sizeof(char));
    }
}
void makechild(pdata p,int n)
{
    int fds[2];
    int i;
    pid_t pid;
    for(i=0;i<n;i++)
    {
        socketpair(AF_LOCAL,SOCK_STREAM,0,fds);
        pid=fork();
        if(pid==0)
        {
            close (fds[0]);
            handle(fds[1]);
        }
        close (fds[1]);
        p[i].pid=pid;
        p[i].fd=fds[0];

    }
}
