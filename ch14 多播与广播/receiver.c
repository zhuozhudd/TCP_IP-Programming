#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 30
void error_handling(char *message);

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <Group IP> <port>\n", argv[0]);
        exit(1);
    }

    int recv_sock;
    int str_len;
    char buf[BUF_SIZE];
    struct sockaddr_in addr;
    struct ip_mreq join_addr;

    recv_sock = socket(PF_INET, SOCK_DGRAM, 0);
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(atoi(argv[2]));

    if (bind(recv_sock, (struct sockaddr *)&addr, sizeof(addr)) == -1)
        error_handling("bind() error");

    join_addr.imr_multiaddr.s_addr = inet_addr(argv[1]); // 初始化多播组地址
    join_addr.imr_interface.s_addr = htonl(INADDR_ANY);  // 初始化待入主机的IP地址。

    // 利用套接字选项IP_ADD_MEMBERSHIP加入多播组。
    setsockopt(recv_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void *)&join_addr, sizeof(join_addr));

    while (1)
    {
        // 通过调用recvfrom 函数接收多播数据。
        // 如果不需要知道传输数据的主机地址信息，可以向recvfrom函数的第五个和第六个参数分别传递NULL和0 
        str_len = recvfrom(recv_sock, buf, BUF_SIZE - 1, 0, NULL, 0);
        if (str_len < 0)
            break;
        buf[str_len] = 0;
        fputs(buf, stdout);
    }
    close(recv_sock);

    return 0;
}

void error_handling(char *message)
{
    fputs((char *)message, stderr);
    fputc('\n', stderr);
    exit(1);
}
