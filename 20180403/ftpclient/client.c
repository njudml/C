#include "head.h"
#include "md5.h"

int main(int argc, char* argv[])
{
    if (argc != 3) {
        printf("error args\n");
        return -1;
    }
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    int ret1;
    struct sockaddr_in ser;
    ser.sin_family = AF_INET;
    ser.sin_addr.s_addr = inet_addr(argv[1]);
    ser.sin_port = htons(atoi(argv[2]));
    ret1 = connect(sfd, (struct sockaddr*)&ser, sizeof(ser));
    if (ret1 == -1) {
        perror("connect");
        close(sfd);
        return -1;
    }
lable1:
    system("clear");
    printf("1.log in\n");
    printf("2.register\n");
    int o;
    int ret;
    char buf[30];
    scanf("%d", &o);
    if (o == 1) {
        bzero(buf, sizeof(buf));
        send(sfd, &o, sizeof(int), 0);
        printf("please input username :");
        scanf("%s", buf);
        send(sfd, buf, sizeof(buf), 0);
        recv(sfd, &ret, sizeof(int), 0);
        if (ret) {
            char* passwd;
            passwd = getpass("please input password:");
            send(sfd, passwd, strlen(passwd), 0);
            recv(sfd, &ret, sizeof(int), 0);
            if (ret)
                printf("welcome\n");
            else {
                printf("wrong password\n");
                return -1;
            }
        }
    } else if (o == 2) {
        bzero(buf, sizeof(buf));
        send(sfd, &o, sizeof(int), 0);
        printf("please input username :");
        scanf("%s", buf);
        send(sfd, buf, sizeof(buf), 0);
        recv(sfd, &ret, sizeof(int), 0);
        if (ret) {
            char* passwd;
            passwd = getpass("please input password:");
            send(sfd, passwd, strlen(passwd), 0);
            recv(sfd, &ret, sizeof(int), 0);
            if (ret)
                printf("register success\n");
        } else {
            printf("username already exist\n");
        }
        goto lable1;
    }
    int i;
    char md5[16] = { 0 };
    int j;
    struct stat st;
    char buf1[1000] = { 0 };
    while (1) {
        bzero(buf1, sizeof(buf1));
        bzero(buf, sizeof(buf));
        bzero(md5, sizeof(md5));
        bzero(&st, sizeof(st));
        scanf("%s", buf);
        if (!strcmp(buf, "ls")) {
            o = 3;
            send(sfd, &o, sizeof(int), 0);
            recv(sfd, buf1, sizeof(buf1), 0);
            puts(buf1);
            continue;
        }
        if (!strcmp(buf, "pwd")) {
            o = 4;
            send(sfd, &o, sizeof(int), 0);
            recv(sfd, buf1, sizeof(buf1), 0);
            puts(buf1);
            continue;
        }
        if (!strcmp(buf, "cd")) {
            o = 5;
            send(sfd, &o, sizeof(int), 0);
            scanf("%s", buf);
            send(sfd, buf, strlen(buf), 0);
            continue;
        }
        if (!strcmp(buf, "puts")) {
            o = 6;
            send(sfd, &o, sizeof(int), 0);
            scanf("%s", buf);
            send(sfd, buf, strlen(buf), 0);
            char signal;
            recv(sfd, &signal, 1, 0);
            MD5_CTX context;
            MD5Init(&context);
            stat(buf, &st);
            int fd = open(buf, O_RDONLY);
            char* data = (char*)malloc(st.st_size);
            read(fd, data, st.st_size);
            MD5Update(&context, data, st.st_size);
            free(data);
            MD5Final(&context, md5);
            lseek(fd, 0, SEEK_SET);
            char realname[33]={0};
            for (int i = 0; i < 16; i++)
                sprintf(realname, "%s%02hhx", realname, md5[i]);
            send(sfd, realname, sizeof(realname), 0);
            struct stat st;
            fstat(fd, &st);
            int sesig;
            recv(sfd, &sesig, sizeof(int), 0);
            if (!sesig) {
                printf("start puts\n");
                send(sfd, &st.st_size, sizeof(off_t), 0);
                sendfile(sfd, fd, 0, st.st_size);
                printf("end puts\n");
            }
            close(fd);
        }
        if (!strcmp(buf, "gets")) {
            o = 7;
            send(sfd, &o, sizeof(int), 0);
            scanf("%s", buf);
            send(sfd, buf, strlen(buf), 0);
            char signal;
            recv(sfd, &signal, 1, 0);
            long len;
            if (-1 == stat(buf, &st))
                len = 0;
            else
                len = st.st_size;
            send(sfd, &len, sizeof(long), 0);
            int fd = open(buf, O_WRONLY | O_CREAT, 0666);
            char buffer[4096] = { 0 };
            lseek(fd, len, SEEK_SET);
            off_t filelen = 0;
            recv(sfd, &filelen, sizeof(off_t), 0);
            printf("start gets\n");
            off_t total = len;
            off_t rec = 0;
            while (total < filelen) {
                rec = 0;
                bzero(buffer, sizeof(buffer));

                rec = recv(sfd, buffer, sizeof(buffer), 0);
                total += rec;
                write(fd, buffer, rec);
            }
            printf("end gets\n");

            close(fd);
        }
        if (!strcmp(buf, "remove")) {
            o = 8;
            send(sfd, &o, sizeof(int), 0);
            scanf("%s", buf);
            send(sfd, buf, strlen(buf), 0);
        }
        if (!strcmp(buf, "mkdir")) {
            o = 9;
            send(sfd, &o, sizeof(int), 0);
            scanf("%s", buf);
            send(sfd, buf, strlen(buf), 0);
        }
    }
}
