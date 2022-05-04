#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <signal.h>

#define BUF_SIZE 30
void error_handling(char *message);
void read_child_proc(int sig);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    // 防止生成僵尸进程
    int state;
    struct sigaction act;
    act.sa_handler = read_child_proc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    state = sigaction(SIGCHLD, &act, 0);

    // socket
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");
    if (listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    // write to file
    int fds[2];
    pid_t pid;
    pipe(fds);
    pid = fork();
    if (pid == 0)
    {
        FILE *fp = fopen("echo_msg.txt", "wt");
        char msg_buf[BUF_SIZE];
        int len;
        for (int i = 0; i < 10; ++i)
        {
            len = read(fds[0], msg_buf, BUF_SIZE);
            fwrite((void *)msg_buf, 1, len, fp);
        }
        fclose(fp);
        return 0;
    }

    // communicating
    int str_len;
    char buf[BUF_SIZE];
    socklen_t clnt_addr_sz;
    while (1)
    {
        clnt_addr_sz = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_sz);
        if (clnt_sock == -1)
            continue;
        else
            puts("New Client Connected...");

        pid = fork();
        if (pid == 0)
        {
            close(serv_sock);
            while ((str_len = read(clnt_sock, buf, BUF_SIZE)) != 0)
            {
                write(clnt_sock, buf, str_len);
                write(fds[1], buf, str_len);
            }
            close(clnt_sock);
            puts("Client disconnected...");
            return 0;
        }
        else
            close(clnt_sock);
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

void read_child_proc(int sig)
{
    int status;
    pid_t id = waitpid(-1, &status, WNOHANG);
    if (WIFEXITED(status))
    {
        printf("Removed proc id: %d\n", id);
        printf("Child send: %d\n", WEXITSTATUS(status));
    }
}
