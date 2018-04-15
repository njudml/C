#include "head.h"
static jmp_buf env;
static void f2 (void)
{
    longjmp(env,2);
}
static void f1(int argc)
{
    if(argc==1)
        longjmp(env,1);
    f2();
}
int main(int argc,char *argv[])
{
    switch(setjmp(env))
    {
    case 0:
        printf("call f1\n");
        f1(argc);
        break;
    case 1:
        printf("jump back from f1\n");
        break;
    case 2:
        printf("jump back from f2\n");
        break;
    }
    return 0;
}
