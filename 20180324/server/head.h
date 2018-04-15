#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <strings.h>
#include <sys/time.h>
#include <sys/select.h>
#include <stdlib.h>
#include <wait.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <setjmp.h>
#include <pthread.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
typedef struct{
    pid_t pid;
    int fd;
    int busy;
}data,*pdata;
typedef struct{
    int len;
    char buf[1000];
}train;
#define FILENAME "file"
void send_fd(int,int);
void recv_fd(int,int *);
void makechild(pdata,int);
void send_file(int );
