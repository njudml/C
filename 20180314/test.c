#include "head.h"
int main()
{
    if(lseek(STDIN_FILENO,0,SEEK_CUR)==-1)
        perror("lseek");
    else 
        printf("OK\n");
    return 0;

}
