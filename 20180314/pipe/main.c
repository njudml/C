#include "head.h"
int main()
{
   int fds[2];
   pipe(fds);
   if(fork()==0)
   {
    close(fds[0]);
    write(fds[1],"hello",5);
   }else
   {
        close(fds[1]);
        wait(NULL);
        char buf[20];
        read(fds[0],buf,sizeof(buf));
        puts(buf);
   }
   return 0;
}
