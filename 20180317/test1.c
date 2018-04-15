#define _BSD_SOURCE
#include <unistd.h>
#include <stdio.h>
int main()
{
   char *p=getpass("name: ");
   puts(p);
   for(;*p!=0;)
       *p++=0;
   puts(p);
}
