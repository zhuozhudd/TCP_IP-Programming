#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char *argv[])
{

    int sock;
    char buf[BUF_SIZE];
    struct sockaddr_in serv_addr;

    FILE *readfp;
    FILE *writefp;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error");
    readfp = fdopen(sock, "r");
    writefp = fdopen(sock, "w");

    while (1)
    {
        // 收到EOF时,fgets函数将返回NULL指针。
        if (fgets(buf, sizeof(buf), readfp) == NULL)
            break;
        fputs(buf, stdout);
        fflush(stdout);
    }
    // 通过该行语句向服务器端发送最后的字符串。
    // 当然，该字符串是在收到服务器端的EOF后发送的。
    fputs("FROM CLIENT: Thank you! \n", writefp); // 此消息没有收到
    fflush(writefp);
    fclose(writefp);
    fclose(readfp);

    return 0;
}

void error_handling(char *message)
{
    fputs((char *)message, stderr);
    fputc('\n', stderr);
    exit(1);
}
