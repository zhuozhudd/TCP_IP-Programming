#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    char message[BUF_SIZE];

    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;
    if (argc != 2)
    {
        printf("Usage : %s <port> \n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");

    if (listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    int str_len;
    clnt_addr_size = sizeof(clnt_addr);
    FILE *readfp;
    FILE *writefp;
    //调用 5 次 accept 函数，共为 5 个客户端提供服务
    for (int i = 0; i < 5; ++i)
    {
        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
        if (clnt_sock == -1)
            error_handling("accept() error");
        else
            printf("Connected client %d \n", i + 1);

        readfp = fdopen(clnt_sock, "r");
        writefp = fdopen(serv_sock, "w");

        while (!feof(readfp))
        {
            fgets(message, BUF_SIZE, readfp);
            fputs(message, writefp);
            // 标准I/O函数为了提高性能，内部提供额外的缓冲。若不调用fflush函数则无法保证立即将数据传输到客户端
            fflush(writefp);
        }

        fclose(readfp);
        fclose(writefp);
    }
    close(serv_sock);
    return 0;
}

void error_handling(char *message)
{
    fputs((char *)message, stderr);
    fputc('\n', stderr);
    exit(1);
}
