#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <fcntl.h>
#include <signal.h>

#define BUF_SIZE 30

void error_handling(char *message);
void urg_handler(int signo);

int acpt_sock;
int recv_sock;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    //
    struct sigaction act;
    act.sa_handler = urg_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    // socket
    struct sockaddr_in recv_addr, serv_addr;
    acpt_sock = socket(PF_INET, SOCK_STREAM, 0);
    socklen_t serv_aadr_sz;

    memset(&recv_addr, 0, sizeof(recv_addr));
    recv_addr.sin_family = AF_INET;
    recv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    recv_addr.sin_port = htons(atoi(argv[1]));
    if (bind(acpt_sock, (struct sockaddr *)&recv_addr, sizeof(recv_addr)) == -1)
        error_handling("bind() error");
    if (listen(acpt_sock, 15) == -1)
        error_handling("listen() error");
    serv_aadr_sz = sizeof(serv_addr);
    recv_sock = accept(acpt_sock, (struct sockaddr *)&serv_addr, &serv_aadr_sz);

    int str_len, state;
    char buf[BUF_SIZE];
    // 文件描述符recv_sock指向的套接字引发的SIGURG信号处理进程变为将getpid函数返回值用作ID的进程
    fcntl(recv_sock, F_SETOWN, getpid());
    // 收到MSG_OOB紧急消息时节操作系统将产生SIGURG信号,并调用注册的信号处理函数。
    state = sigaction(SIGURG, &act, 0);
    while ((str_len = recv(recv_sock, buf, sizeof(buf), 0)) != 0)
    {
        if(str_len == 1)
            continue;
        buf[str_len] = 0;
        puts(buf);
    }
    close(recv_sock);
    close(acpt_sock);
    return 0;
}

void error_handling(char *message)
{
    fputs((char *)message, stderr);
    fputc('\n', stderr);
    exit(1);
}

void urg_handler(int signo)
{
    int str_len;
    char buf[BUF_SIZE];
    str_len = recv(recv_sock, buf, sizeof(buf) - 1, MSG_OOB);
    buf[str_len] = 0;
    printf("Urgent message: %s\n", buf);
}
