#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024
#define OP_SIZE 4

void error_handling(char *message);
int calculate(int op_num, int operands[], char op);

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    if (argc != 2)
    {
        printf("Usage: %s <port> \n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");
    if (listen(serv_sock, 5) == -1)
        error_handling("listend() error");

    char op_info[BUF_SIZE];
    int clnt_addr_sz = sizeof(clnt_addr);
    int operand_cnt, recv_len, recv_cnt, result;
    for (int i = 0; i < 5; ++i)
    {
        operand_cnt = 0;
        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_sz);
        read(clnt_sock, &operand_cnt, 1);

        recv_len = 0;
        while ((operand_cnt * OP_SIZE + 1) > recv_len)
        {
            recv_cnt = read(clnt_sock, &op_info[recv_len], BUF_SIZE - 1);
            recv_len += recv_cnt;
        }
        result = calculate(operand_cnt, (int *)op_info, op_info[recv_len - 1]);
        write(clnt_sock, (char *)&result, sizeof(result));
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

int calculate(int op_num, int operands[], char op)
{
    int result = operands[0];
    switch (op)
    {
    case '+':
        for (int i = 1; i < op_num; ++i)
            result += operands[i];
        break;
    case '*':
        for (int i = 1; i < op_num; ++i)
            result *= operands[i];
        break;
    case '-':
        for (int i = 1; i < op_num; ++i)
            result -= operands[i];
        break;
    }
    return result;
}