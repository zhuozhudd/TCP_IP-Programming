#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>

void error_handling(char *message);

// inet_aton的功能和inet_addr基本相同

int main(int argc, char *argv[])
{
    char *addr = "127.232.124.79";
    struct sockaddr_in addr_inet;

    if (!inet_aton(addr, &addr_inet.sin_addr))
        error_handling("inet_aton() error");
    else
        printf("Network ordered integer addr：%#x \n", addr_inet.sin_addr.s_addr);
    return 0;
}

void error_handling(char *message)
{
    fputs((char *)message, stderr);
    fputc('\n', stderr);
    exit(1);
}