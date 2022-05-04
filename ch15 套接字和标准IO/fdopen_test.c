#include <stdio.h>
#include <fcntl.h>

int main(void)
{
    FILE *fp;
    int fd = open("data.dat", O_WRONLY | O_CREAT | O_TRUNC);
    if (fd == -1)
    {
        fputs("file open error", stdout);
        return -1;
    }
    fp = fdopen(fd, "w");  //返回 写 模式的 FILE 指针
    fputs("Network C programing \n", fp);
    fclose(fp);
    return 0;
}
