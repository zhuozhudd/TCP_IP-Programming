#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);

int main(int argc, char *argv[])
{
    int sock;
    int send_buf = 1024 * 4, recv_buf = 1024 * 4;
    int state;
    socklen_t len;

    sock = socket(PF_INET, SOCK_STREAM, 0);

    // 设置 SENDBUF和RECVBUF 缓冲大小
    state = setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void *)&send_buf, sizeof(send_buf));
    if (state)
        error_handling("getsockopt() error");
    state = setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void *)&recv_buf, sizeof(recv_buf));
    if (state)
        error_handling("getsockopt() error");
    
    // 查看设置后的缓冲大小
    len = sizeof(send_buf);
    state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void *)&send_buf, &len);
    if (state)
        error_handling("getsockopt() error");
    len = sizeof(recv_buf);
    state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void *)&recv_buf, &len);
    if (state)
        error_handling("getsockopt() error");
    printf("Input buffer size: %d \n", recv_buf);
    printf("Output buffer size: %d \n", send_buf);
    return 0;

    return 0;
}

void error_handling(char *message)
{
    fputs((char *)message, stderr);
    fputc('\n', stderr);
    exit(1);
}
