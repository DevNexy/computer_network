#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/un.h>
#include <fcntl.h>

#define BUF_SIZE 1024
#define SOCK_ADDR "./sock_addr"

int main()
{
	int sock, len;
	struct sockaddr_un sock_addr;
	char buf[BUF_SIZE] = {
		0,
	};
	sock_addr.sun_family = AF_UNIX;
	strcpy(sock_addr.sun_path, SOCK_ADDR);

	sock = socket(AF_UNIX, SOCK_STREAM, 0);

	if (sock == -1)
	{
		perror("socket error");
	}
	if (connect(sock, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) == -1)
	{
		perror("connext error");
		exit(1);
	}

	while (1)
	{
		
		printf("message : ");
		gets(buf);

		if (send(sock, buf, BUF_SIZE + 1, 0) == -1)
		{
			perror("send error\n");
			exit(1);
		}

		if (strncmp(buf, "quit", strlen("quit")) == 0)
		{
			break;
		}

		memset(buf, 0, BUF_SIZE);
	}

	close(sock);

	return 0;
}