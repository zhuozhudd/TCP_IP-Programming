#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 30
void error_handling(char *message);

// UDP是具有数据边界的协议，传输中调用I/O函数的次数非常重要
// 输入函数的调用次数应和输出函数的调用次数完全一致
int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in my_addr, your_addr;

    if (argc != 2)
    {
        printf("Usage: %s <port> \n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == -1)
        error_handling("socket() error");

    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    my_addr.sin_port = htons(atoi(argv[1]));

    if (bind(sock, (struct sockaddr *)&my_addr, sizeof(my_addr)) == -1)
        error_handling("bind() error");

    char message[BUF_SIZE];
    socklen_t addr_sz;
    int str_len;
    for (int i = 0; i < 3; ++i)
    {
        sleep(5);
        addr_sz = sizeof(your_addr);
        str_len = recvfrom(sock, message, BUF_SIZE, 0, (struct sockaddr *)&your_addr, &addr_sz);
        printf("Message %d: %s\n", i + 1, message);
    }
    close(sock);
    return 0;
}

void error_handling(char *message)
{
    fputs((char *)message, stderr);
    fputc('\n', stderr);
    exit(1);
}
