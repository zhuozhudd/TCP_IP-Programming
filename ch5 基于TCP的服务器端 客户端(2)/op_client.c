#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define RESULT_SIZE 4
#define OP_SIZE 4
void error_handling(char *message);

int main(int argc, char *argv[])
{
    int sock;
    char op_msg[BUF_SIZE];
    struct sockaddr_in serv_addr;
    if (argc != 3)
    {
        printf("Usage: %s <IP> <port> \n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error");
    else
        printf("Connected...");

    int result, operand_cnt;
    fputs("Operand count: ", stdout);
    scanf("%d", &operand_cnt);
    op_msg[0] = (char)operand_cnt;

    for (int i = 0; i < operand_cnt; ++i)
    {
        printf("Operand %d: ", i + 1);
        scanf("%d", (int *)&op_msg[i * OP_SIZE + 1]);
    }
    fgetc(stdin); // 删除缓冲中的字符\n
    fputs("Operator: ", stdout);
    scanf("%c", &op_msg[operand_cnt * OP_SIZE + 1]);

    /*
    printf("%d\n", (char)op_msg[0]);
    printf("%d\n", (int)op_msg[1]);
    printf("%d\n", (int)op_msg[5]);
    printf("%d\n", (int)op_msg[9]);
    puts(&op_msg[operand_cnt * OP_SIZE + 1]);
    */
    printf("%c\n", op_msg[operand_cnt * OP_SIZE + 1]);

    write(sock, op_msg, operand_cnt * OP_SIZE + 2);
    read(sock, &result, RESULT_SIZE);
    printf("Operation result: %d \n", result);
    close(sock);

    return 0;
}

void error_handling(char *message)
{
    fputs((char *)message, stderr);
    fputc('\n', stderr);
    exit(1);
}
