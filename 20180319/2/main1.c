#include "head.h"
#include <errno.h>
int main()
{
    for(int i=1;i<133;i++)
    {
    errno=i;
    perror(" ");
    }
}
