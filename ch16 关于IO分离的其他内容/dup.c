#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int cp_fd1, cp_fd2;
    char str1[] = "Hi~ \n";
    char str2[] = "It's nice day~ \n";

    cp_fd1 = dup(1);
    cp_fd2 = dup2(cp_fd1, 7);

    printf("fd1 = %d, fd2 = %d \n", cp_fd1, cp_fd2);
    write(cp_fd1, str1, sizeof(str1));  // 利用复制出的文件描述符进行输出。
    write(cp_fd2, str2, sizeof(str2));  // 该输出结果可以验证是否进行了实际复制。

    close(cp_fd1);
    close(cp_fd2);
    write(1, str1, sizeof(str1)); //  终止复制的文件描述符。但仍有1个描述符，因此可以进行输出。
    close(1); // 终止最后的文件描述符，因此无法完成下一行的输出。
    write(1, str2, sizeof(str2)); 
    return 0;
}