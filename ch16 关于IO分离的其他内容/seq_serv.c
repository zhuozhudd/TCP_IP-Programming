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
    int serv_sock, clnt_sock;
    FILE *readfp;
    FILE *writefp;

    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_sz;
    char buf[BUF_SIZE] = {0};
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))== -1)
        error_handling("bind() error");
    if(listen(serv_sock, 5)== -1)
        error_handling("listen() error");
    clnt_addr_sz = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_sz);

    // 通过clnt_sock中保存的文件描述符创建读模式FILE指针和写模式FILE指针。
    readfp = fdopen(clnt_sock, "r");
    writefp = fdopen(clnt_sock, "w");

    // 向客户端发送字符串，调用fflush函数结束发送过程。
    fputs("FROM SERVER: Hi~ client?\n", writefp);
    fputs("I love all of the world \n", writefp);
    fputs("You are awesome! \n", writefp);
    fflush(writefp);

    // 针对写模式FILE指针调用fclose函数。
    // 调用fclose 函数终止套接字时，对方主机将收到EOF
    fclose(writefp); 
    fgets(buf, sizeof(buf), readfp);
    fputs(buf, stdout);
    fclose(readfp);

    return 0;
}


void error_handling(char *message)
{
    fputs((char *)message, stderr);
    fputc('\n', stderr);
    exit(1);
}
