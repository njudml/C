#include "head.h"
int main()
{
    pid_t pid;
    pid=fork();
    if(pid==0)
    {
        return 5;
    }
    else
    {
        pid_t cpid;
        int status;
        cpid=wait(&status);
        printf("cpid=%d\n",cpid);
        if(WIFEXITED(status))
        {
            printf("%d\n",WEXITSTATUS(status));
        }
    }
    return 0;
}
