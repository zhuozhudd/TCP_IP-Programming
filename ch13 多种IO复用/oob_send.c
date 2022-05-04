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
    if (argc!=3)
    {
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    int sock;
    struct sockaddr_in recv_addr;
    sock = socket(PF_INET, SOCK_STREAM, 0);
    recv_addr.sin_family = AF_INET;
    recv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    recv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock,(struct sockaddr*)&recv_addr,sizeof(recv_addr))==-1)
        error_handling("connect() error!");

    // 传输数据。第32和第34行紧急传输数据。
    // 正常顺序应该是123,4,567,890,但紧急传输了4和890,由此可知接收顺序也将改变。
    write(sock, "123", strlen("123"));
    send(sock, "4", strlen("4"), MSG_OOB);
    write(sock, "567", strlen("567"));
    send(sock, "890", strlen("890"), MSG_OOB);
    close(sock);

    return 0;
}


void error_handling(char *message)
{
    fputs((char *)message, stderr);
    fputc('\n', stderr);
    exit(1);
}
