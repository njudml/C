#include "head.h"
int main()
{
    int fd;
    fd=open("file",O_RDWR);
    if(fd==-1)
    {
        perror("open");
        return -1;
    }
    write(fd,"hello",5);
    close(fd);
    return 0;
}
