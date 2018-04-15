#define _GNU_SOURCE
#include <sys/utsname.h>
#include "head.h"
int main(int argc,char *argv[])
{
    struct utsname uts;
    uname(&uts);
    printf("%s\n",uts.nodename);
    printf("%s\n",uts.sysname);
    printf("%s\n",uts.release);
    printf("%s\n",uts.version);
    printf("%s\n",uts.machine);
    printf("%s\n",uts.domainname);
}
