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
#define SOCK_ADDR "./sock_addr_client"
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

	int sock, len;
	struct sockaddr_in s_addr;
	struct sockaddr sock_addr;
	s_addr.sin_family = AF_INET;
	s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	s_addr.sin_port = PORT;

	sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock == -1)
	{
		printf("error");
	}
	if (connect(sock, (struct sockaddr *)&s_addr, sizeof(s_addr)) == -1)
	{
		perror("connext error");
		exit(1);
	}

	int flag_client_input = fcntl(c_input, F_GETFL, 0);
	if (fcntl(c_input, F_SETFL, flag_client_input | O_NONBLOCK) == -1)
	{
		printf("error");
	}

	int flag_client = fcntl(sock, F_GETFL, 0);
	fcntl(sock, F_SETFL, flag_client | O_NONBLOCK);
	int flag_end = 0;
	while (1)
	{
		sleep(1);
		memset(buf, 0, BUFSIZ);
		if (recv(c_input, buf, BUFSIZ + 1, 0) != -1)

			sleep(1);
		if (send(sock, buf, BUFSIZ, 0) && strlen(buf))
		{
			printf("[ME] : %s \n", buf);
			if (strncmp(buf, "quit", strlen("quit")) == 0)
			{
				flag_end = 1;
			}
			memset(buf, 0, BUFSIZ);
		}

		sleep(1);
		if (recv(sock, buf, BUFSIZ, 0) && strlen(buf))
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
	return 0;
}
