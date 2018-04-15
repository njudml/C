#include "head.h"
int main(int argc,char *argv[])
{
    if(argc!=3)
    {
        printf("error args\n");
        return -1;
    }
    int sfd=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in ser;
    ser.sin_family=AF_INET;
    ser.sin_addr.s_addr=inet_addr(argv[1]);
    ser.sin_port=htons(atoi(argv[2]));
    connect(sfd,(struct sockaddr*)&ser,sizeof(ser));
    int len;
    char buf[1000]={0};
    recv(sfd,&len,sizeof(int),0);
    recv(sfd,buf,len,0);
    int fd=open(buf,O_WRONLY|O_CREAT,0666);
    while(1)
    {
        recv(sfd,&len,sizeof(len),0);
        if(len>0)
        {
            recv_n(sfd,buf,len);
            write(fd,buf,sizeof(buf));

        }else
            break;
    }
    close(fd);
    close(sfd);
}
