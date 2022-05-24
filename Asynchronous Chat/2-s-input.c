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
int main()
{
	int sock, len;
	struct sockaddr_un sock_addr;
	char buf[BUFSIZ] = {0,};
	sock_addr.sun_family = AF_UNIX;
	strcpy(sock_addr.sun_path, SOCK_ADDR);
	sock = socket(AF_UNIX, SOCK_STREAM, 0);

	if (sock == -1)
	{
		perror("Socket error\n");
	}
	if (connect(sock, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) == -1)
	{
		perror("Connect error\n");
		exit(1);
	}
	while (1)
	{
		printf("message : ");
		gets(buf);

		if (send(sock, buf, BUFSIZ + 1, 0) == -1)
		{
			perror("Send error\n");
			exit(1);
		}

		if (strncmp(buf, "quit", strlen("quit")) == 0)
		{
			break;
		}
		memset(buf, 0, BUFSIZ);
	}
	close(sock);
	return 0;
}
