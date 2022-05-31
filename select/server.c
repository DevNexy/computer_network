#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <signal.h>
#include <netinet/in.h>
#include <fcntl.h>

#define IP "127.0.0.1"
#define BUFSIZ 1024
#define PORTNUM 9001
#define COUNT 3
int getMaxInt(int* n, int size) {
  int max = n[0];

  for (int i = 1; i < size; i++)
    if (n[i] > max) max = n[i];

  return max;
}
int main()
{
    fd_set readfds;
    int server, len, ret;
    int client[COUNT];
    char msg[50];
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    char buf[BUFSIZ] = {0,};

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(IP);
    server_addr.sin_port = PORTNUM;
    server = socket(AF_INET, SOCK_STREAM, 0);

    if (server == -1)
    {
        perror("Server Error\n");
        exit(1);
    }
    if (bind(server, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Bind Error\n");
        exit(1);
    }
    if (listen(server, 5))
    {
        printf("Error");
        perror("Listen Error\n");
        exit(3);
    }
    sleep(5);
    for (int i = 0; i < COUNT; i++)
    {
        if ((client[i] = accept(server, (struct sockaddr *)&client_addr, &len)) == -1)
        {
            perror("Accept");
            break;
        }
        else
        {
            printf("Client %d Connected.\n", i+1);
            
        }    
    }
    while (1)
    {
        usleep(500);
        FD_ZERO(&readfds);
        for (int i = 0; i < COUNT; i++)
        {
            FD_SET(client[i], &readfds);
        }
        
        ret = select(getMaxInt(client , COUNT)+1 , &readfds, NULL, NULL, NULL);
        
        switch (ret)
        {
        case -1:
            perror("Error");
        case 0:
            printf("Select Returns : 0\n");
            break;
        default:
            int kk = 0;
            while (ret > 0)
            {
                if (FD_ISSET(client[kk], &readfds))
                {
                    memset(buf, 0, BUFSIZ);
                    if (recv(client[kk], buf, BUFSIZ, 0) == -1)
                    {
                        perror("Recv");
                        break;
                    }
                    ret--;
                    printf("\nMSG from client %d: %s \n", kk+1, buf);
                }else;
                kk++;
            }
            break;
        }
    }
    return 0;
}