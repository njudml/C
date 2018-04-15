#include "head.h"
int main(int argc,char* argv[])
{
    if(argc!=3)
    {
        perror("args error");
        return -1;
    }
    int fdr,fdw;
    fdw=open(argv[1],O_WRONLY);
    if(fdw==-1)
    {
        perror("open");
        return -1;
    }
    fdr=open(argv[2],O_RDONLY);
    if(fdr==-1)
    {
        perror("open");
        return -1;
    }
    printf("fdr=%d,fdw=%d\n",fdr,fdw);
    char buf[500]={0};
    int ret;
    fd_set set;
    while(1)
    {
        FD_ZERO(&set);
        FD_SET(0,&set);
        FD_SET(fdr,&set);
        ret=select(fdr+1,&set,NULL,NULL,NULL);
        if(ret>0)
        {
            if(FD_ISSET(0,&set))
            {
                bzero(buf,sizeof(buf));
                ret=read(0,buf,sizeof(buf)-1);
                if(ret==0)
                    break;
                write(fdw,buf,strlen(buf)-1);
            }
            if(FD_ISSET(fdr,&set))
            {
                bzero(buf,sizeof(buf));
                ret=read(fdr,buf,sizeof(buf));
                if(ret==0)
                    break;
                printf("%s\n",buf);
            }
        }
    }
    close(fdr);
    close(fdw);
    return 0;
}
