#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>

#define BUF_SIZE 100
void error_handling(char *message);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

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

    // select
    fd_set reads, cpy_reads;
    struct timeval timeout;
    socklen_t clnt_addr_sz;
    int fd_max, fd_num;

    /**
     * 向要传到select函数第二个参数的fd_set变量reads注册服务器端套接字。
     * 这样，接收数据情况的监视对象就包含了服务器端套接字。客户端的连接请求同样通过传输数据完成。
     * 因此，服务器端套接字中有接收的数据，就意味着有新的连接请求。
     */
    FD_ZERO(&reads);
    FD_SET(serv_sock, &reads);
    fd_max = serv_sock;

    char buf[BUF_SIZE];
    int str_len;

    while (1)
    {
        cpy_reads = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 5000;
        
        // 在while无限循环中调用select函数。select 函数的第三和第四个参数为空。
        // 只需根据监视目的传递必要的参数。
        if ((fd_num = select(fd_max + 1, &cpy_reads, 0, 0, &timeout)) == -1)
            break;

        if (fd_num = 0)
            continue;
        
        // select函数返回大子等于1 的值时执行的循环。
        for (int i = 0; i < fd_max + 1; ++i)
        {
            // 查找发生状态变化的(有接收数据的套接字的)文件描述符。
            if (FD_ISSET(i, &cpy_reads))
            {
                // 发生状态变化时，首先验证服务器端套接字中是否有变化。
                if (i == serv_sock)
                {
                    // 如果是服务器端套接字的变化，将受理连接请求
                    clnt_addr_sz = sizeof(clnt_addr);
                    clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_sz);
                    // 在reads中注册了与客户端连接的套接字文件描述符。
                    FD_SET(clnt_sock, &reads);
                    if (fd_max < clnt_sock)
                        fd_max = clnt_sock;
                    printf("connected client: %d\n", clnt_sock);
                }
                else  // 发生变化的套接字并非服务器端套接字时，即有要接受的数据时执行else语句。
                {
                    str_len = read(i, buf, BUF_SIZE);
                    // 但此时需要确认接收的数据是字符串还是代表断开连接的EOF 。
                    if (str_len == 0)
                    {
                        // 接收的数据为EOF时需要关闭套接字，并从reads 中删除相应信息。
                        FD_CLR(i, &reads);
                        close(i);
                        printf("closed client: %d\n", i);
                    }
                    else  // 接收的数据为字符串时，执行回声服务。
                        write(i, buf, str_len);
                }
            }
        }
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
