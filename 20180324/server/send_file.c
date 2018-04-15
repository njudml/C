#include "head.h"
void send_n(int sfd,char *buf,int len)
{
    int total=0;
    int ret;
    while(total<len)
    {
        ret=send(sfd,buf+total,len-total,0);
        total+=ret;
    }
}
void recv_n(int sfd,char *buf,int len)
{
    int total=0;
    int ret;
    while(total<len)
    {
        ret=send(sfd,buf+total,len-total,0);
        total+=ret;
    }
}
void send_file(int sfd)
{
    train t;
    strcpy(t.buf,FILENAME);
    t.len=strlen(t.buf);
    send(sfd,&t,4+t.len,0);
    int fd=open(FILENAME,O_RDONLY);
    while((t.len=read(fd,t.buf,sizeof(t.buf)))>0)
    {
        send_n(sfd,(char*)&t,4+t.len); 
    }
    bzero(&t,sizeof(t));
    send_n(sfd,(char *)&t,4+t.len);
    close(sfd);
}

