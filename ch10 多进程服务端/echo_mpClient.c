#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 30

void error_handling(char *message);
void read_routine(int sock, char *buf);
void write_routine(int sock, char *buf);
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    // socket
    int sock;
    struct sockaddr_in serv_addr;
    sock = socket(PF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error");

    // read & write
    char buf[BUF_SIZE];
    pid_t pid;
    pid = fork();
    if (pid == 0)
        write_routine(sock, buf);
    else
        read_routine(sock, buf);
    close(sock);
    return 0;
}

void read_routine(int sock, char *buf)
{
    while (1)
    {
        int str_len = read(sock, buf, BUF_SIZE);
        if (str_len == 0)
            return;
        buf[str_len] = 0;
        printf("Message from server: %s", buf);
    }
}

void write_routine(int sock, char *buf)
{
    while (1)
    {
        fgets(buf, BUF_SIZE, stdin);
        if (!strcmp(buf, "q\n") || !strcmp(buf, "Q\n"))
        {
            /**
             * 调用shutdown函数向服务器传递EOF。
             * 当执行下一句return;时，可以调用39行的close函数传递EOF
             * 但现在已通过fork函数调用复制文件描述符，此时无法通过1次close函数调用传递EOF
             * 因此需要通过shutdown函数调用另外传递
             */
            shutdown(sock, SHUT_WR);
            return;
        }
        write(sock, buf, strlen(buf));
    }
}

void error_handling(char *message)
{
    fputs((char *)message, stderr);
    fputc('\n', stderr);
    exit(1);
}
