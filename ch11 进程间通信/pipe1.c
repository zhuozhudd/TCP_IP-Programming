#include <stdio.h>
#include <unistd.h>
#define BUF_SIZE 30

// 管道单向通信，子进程仅用于输入路径，父进程仅用输出路径
int main(int argc, char* argv[])
{
    int fds[2];
    char str[] = "Who are you?";
    char buf[BUF_SIZE];
    pid_t pid;

    pipe(fds);
    // 调用fork函数，子进程将同时拥有pipe(fds)函数调用获取的2个文件描述符。
    // 注意，复制的并非管道，而是用于管道IO的文件描述符
    pid = fork();
    if (pid == 0)
    {
        // 子进程通过fds[1]向管道传递字符串，fds[1]即管道入口
        write(fds[1], str, sizeof(str));
    }
    else
    {
        // 父进程通过fds[0]从管道接收字符串，fds[0]即管道出口
        read(fds[0], buf, BUF_SIZE);
        puts(buf);
    }
    return 0;
}