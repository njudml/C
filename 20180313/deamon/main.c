#include "head.h"
int main()
{
    pid_t pid;
    pid=fork();
    if(pid>0)
        exit(0);
    setsid();
    chdir("/");
    umask(0);
    for(int i=0;i<3;i++)
        close(i);
    int fd;
    fd=open("/tmp/log",O_RDWR|O_APPEND|O_CREAT);
    char buf[30]="I have sleep 5 seconds\n";
    while(1)
    {
        sleep(5);
        write(fd,buf,strlen(buf));
    }
    return 0;
}
