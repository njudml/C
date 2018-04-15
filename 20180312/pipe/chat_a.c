#include "head.h"
int main(int argc,char*argv[])
{
    if(argc!=3)
    {
        perror("args error");
        return -1;
    }
    int fdr=open(argv[1],O_RDONLY);
    if(fdr==-1)
    {
        perror("open");
        return -1;
    }
    int fdw=open(argv[2],O_WRONLY);
    if(fdw==-1)
    {
        perror("open");
        return -1;
    }
    printf("fdr=%d,fdw=%d\n",fdr,fdw);
    char buf[512]={0};
    int ret;
    fd_set readfds;
    while(1)
    {
        FD_ZERO(&readfds);
        FD_SET(0,&readfds);
        FD_SET(fdr,&readfds);
        ret=select(fdr+1,&readfds,NULL,NULL,NULL);
        if(ret>0)
        {
            if(FD_ISSET(fdr,&readfds))
            {
                bzero(buf,sizeof(buf));
                ret=read(fdr,buf,sizeof(buf));
                if(ret==0)
                    break;
                printf("%s\n",buf);
            }
            if(FD_ISSET(0,&readfds))
            {
                bzero(buf,sizeof(buf));
                ret=read(0,buf,sizeof(buf)-1);
                if(ret==0)
                    break;
                write(fdw,buf,strlen(buf)-1);
            }
        }

    }
    close(fdr);
    close(fdw);
    return 0;

}
