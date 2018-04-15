#include "head.h"
int main()
{
    pid_t pid=fork();
    if(pid==0)
    {
        return 0;
    }else
    {
        while(1);
    }
    return 0;
}
