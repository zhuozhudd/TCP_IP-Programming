#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void read_child_proc(int sig)
{
    int status;
    pid_t id = waitpid(-1, &status, WNOHANG);
    if(WIFEXITED(status))
    {
        printf("Removed proc id: %d\n", id);
        printf("Child send: %d\n", WEXITSTATUS(status));
    }
}

int main(int argc, char* argv[])
{
    pid_t pid;

    /**
     * @brief 
     * 注册SIGCHLD信号对应的处理器。
     * 若子进程终止，则调用read_child_proc函数。
     * 处理函数中调用了waitpid函数，所以子进程正常终止，不会成为僵尸进程
     */
    struct sigaction act;
    act.sa_handler = read_child_proc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0);

    pid = fork();
    if(pid == 0)
    {
        puts("Hi! I'm child process");
        sleep(10);
        return 12;
    }
    else
    {
        printf("Child proc id: %d\n", pid);
        pid = fork();
        if(pid == 0)
        {
            puts("Hi! I'm child process");
            sleep(10);
            exit(24);
        }
        else
        {
            printf("Child proc id :%d\n", pid);
            for (int i = 0; i < 5; ++i)
            {
                puts("wait...");
                sleep(5);
            }
        }
    }

    return 0;
}