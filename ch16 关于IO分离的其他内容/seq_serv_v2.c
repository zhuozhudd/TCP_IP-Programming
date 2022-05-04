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

    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");
    if (listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    clnt_addr_sz = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_sz);

    readfp = fdopen(clnt_sock, "r");
    writefp = fdopen(dup(clnt_sock), "w");

    fputs("FROM SERVE: Hi~ client? \n", writefp);
    fputs("I love all of the world \n", writefp);
    fputs("You are awesome! \n", writefp);
    fflush(writefp);

    // 
    shutdown(fileno(writefp), SHUT_WR); // fileno将writefp指针转为文件描述符
    fclose(writefp);
    printf("-------after shutdown------\n");
    fgets(buf, sizeof(buf), readfp);
    fputs(buf, stdout);

    return 0;
}

void error_handling(char *message)
{
    fputs((char *)message, stderr);
    fputc('\n', stderr);
    exit(1);
}
