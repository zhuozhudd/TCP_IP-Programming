#include<stdio.h>
#include<sys/uio.h>

int main(int argc, char *argv[])
{
    struct iovec vec[2];
    char buf1[] = "ABCDEFG";
    char buf2[] = "1234567";
    int str_len;
    vec[0].iov_base = buf1;
    vec[0].iov_len = 3;
    vec[1].iov_base = buf2;
    vec[1].iov_len = 4;
    // writev的第一个参数1是文件描述符，因此向控制台输出数据， 
    // vec是存有待发送数据信息的iovec数组指针。
    // 第三个参数为2 ，因此，从ptt指向的地址开始，共浏览2个iovec结构体变量，发送这些指针指向的缓冲数据。
    str_len = writev(1, vec, 2);
    puts("");
    printf("Write bytes: %d\n", str_len);
    return 0;
}