#include <inttypes.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


#define IP "127.0.0.1"
#define BUFSIZ 1024
#define PORTNUM 9001

void handler(int sig)
{
    close(0);
}

int main()
{
    int sock, len;
    struct sockaddr_in server_addr;
    struct sockaddr sock_addr;
    char buf[BUFSIZ] = {0,};

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(IP);
    server_addr.sin_port = PORTNUM;
    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == -1)
    {
        printf("Error");
    }
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Connect Error");
        exit(1);
    }
    while (1)
    {
        memset(buf, 0, BUFSIZ);
        printf("Message : ");
        gets(buf);
        if (strncmp(buf, "quit", strlen("quit")) == 0)
        {
            break;
        }
        if (send(sock, buf, BUFSIZ, 0) == -1)
        {
            perror("Send");
            exit(1);
        }
    }
    return 0;
}
