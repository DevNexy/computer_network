#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>

#define BUFSIZ 1024
#define SOCK_ADDR "./sock_addr"
#define PORT 5088

int main()
{
    int s_input, c_input, len_input;
    struct sockaddr_un s_input_addr;
    struct sockaddr c_input_addr;
    char buf[BUFSIZ];
    s_input_addr.sun_family = AF_UNIX;
    strcpy(s_input_addr.sun_path, SOCK_ADDR);

    s_input = socket(AF_UNIX, SOCK_STREAM, 0);

    if (s_input == -1)
    {
        perror("Sever error\n");
        exit(1);
    }
    if (bind(s_input, (struct sockaddr *)&s_input_addr, sizeof(s_input_addr)) == -1)
    {
        perror("Bind error\n");
        exit(1);
    }
    if (listen(s_input, 5) == -1)
    {
        perror("Listen Error\n");
        exit(3);
    }
    len_input = sizeof(c_input_addr);
    c_input = accept(s_input, &c_input_addr, &len_input);

    int s, c, len;
    struct sockaddr_in s_addr;
    struct sockaddr c_addr;
    int cnt = 0;

    s_addr.sin_family = AF_INET;
    s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    s_addr.sin_port = PORT;

    s = socket(AF_INET, SOCK_STREAM, 0);

    if (s == -1)
    {
        perror("Sever error\n");
        exit(1);
    }

    if (bind(s, (struct sockaddr *)&s_addr, sizeof(s_addr)) == -1)
    {
        perror("Bind error\n");
        exit(1);
    }

    if (listen(s, 5) == -1)
    {
        printf("Error\n");
        perror("Listen Error\n");
        exit(3);
    }

    len = sizeof(c_addr);
    c = accept(s, &c_addr, &len);

    int flag_client_input = fcntl(c_input, F_GETFL, 0);
    fcntl(c_input, F_SETFL, flag_client_input | O_NONBLOCK);

    int flag_client = fcntl(c, F_GETFL, 0);
    fcntl(c, F_SETFL, flag_client | O_NONBLOCK);

    int flag_end = 0;
    while (1)
    {
        sleep(1);
        memset(buf, 0, BUFSIZ);
        if (recv(c_input, buf, BUFSIZ + 1, 0) != -1)
            sleep(1);
        if (send(c, buf, BUFSIZ, 0) && strlen(buf))
        {
            printf("[ME] : %s \n", buf);
            if (strncmp(buf, "quit", strlen("quit")) == 0)
            {
                flag_end = 1;
            }
            memset(buf, 0, BUFSIZ);
        }
        sleep(1);
        if (recv(c, buf, BUFSIZ, 0) && strlen(buf))
        {
            printf("[YOU] : %s \n", buf);

            if (strncmp(buf, "quit", strlen("quit")) == 0 && flag_end == 1)
            {
                exit(1);
            }
            memset(buf, 0, BUFSIZ);
        }
    }

    close(s_input);
    close(c_input);
    close(s);
    close(c);
    return 0;
}
