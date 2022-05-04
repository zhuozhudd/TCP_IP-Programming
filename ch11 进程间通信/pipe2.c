#include <stdio.h>
#include <unistd.h>
#define BUF_SIZE 30

// 单管道双向通信
int main(int argc, char *argv[])
{
    int fds[2];
    char str1[] = "Who are you?";
    char str2[] = "Thank you for your message";
    char buf[BUF_SIZE];
    pid_t pid;

    pipe(fds);
    pid = fork();
    if (pid == 0)
    {
        // 1进0出
        write(fds[1], str1, sizeof(str1));
        sleep(2); // 注释此行会发生错误，子进程毁直接读回自己在上一行向管道发送的数据
        read(fds[0], buf, BUF_SIZE);
        printf("Child proc output: %s \n", buf);
    }
    else
    {
        read(fds[0], buf, BUF_SIZE);
        printf("Parent proc output: %s\n", buf);
        write(fds[1], str2, sizeof(str2));
        sleep(3);
    }

    return 0;
}