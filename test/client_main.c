#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>

#define BUF_SIZE 1024
#define SOCK_ADDR "./sock_addr_client"
#define SERVER_IP "127.0.0.1"
#define PORT 5088
int main()
{

	int server_input, client_input, len_input;
	struct sockaddr_un server_input_addr;
	struct sockaddr client_input_addr;
	char buf[BUF_SIZE];

	server_input_addr.sun_family = AF_UNIX;
	strcpy(server_input_addr.sun_path, SOCK_ADDR);

	server_input = socket(AF_UNIX, SOCK_STREAM, 0);

	if (server_input == -1)
	{
		perror("Sever error\n");
		exit(1);
	}
	if (bind(server_input, (struct sockaddr *)&server_input_addr, sizeof(server_input_addr)) == -1)
	{
		perror("Bind error\n");
		exit(1);
	}
	if (listen(server_input, 5) == -1)
	{
		perror("Listen Error\n");
		exit(3);
	}
	len_input = sizeof(client_input_addr);
	client_input = accept(server_input, &client_input_addr, &len_input);

	int sock, len;
	struct sockaddr_in server_addr;
	struct sockaddr sock_addr;

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
	server_addr.sin_port = PORT;

	sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock == -1)
	{
		printf("error");
	}
	if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
	{
		perror("connext error");
		exit(1);
	}

	int flag_input_client = fcntl(client_input, F_GETFL, 0);
	if (fcntl(client_input, F_SETFL, flag_input_client | O_NONBLOCK) == -1)
	{
		printf("error");
	}

	int flag_client = fcntl(sock, F_GETFL, 0);
	fcntl(sock, F_SETFL, flag_client | O_NONBLOCK);
	int end_flag = 0;
	while (1)
	{
		sleep(1);
		memset(buf, 0, BUF_SIZE);
		if (recv(client_input, buf, BUF_SIZE + 1, 0) != -1)

			sleep(1);
		if (send(sock, buf, BUF_SIZE, 0) && strlen(buf))
		{
			printf("[ME] : %s \n", buf);
			if (strncmp(buf, "quit", strlen("quit")) == 0)
			{
				end_flag = 1;
			}
			memset(buf, 0, BUF_SIZE);
		}

		sleep(1);
		if (recv(sock, buf, BUF_SIZE, 0) && strlen(buf))
		{
			printf("[YOU] : %s \n", buf);
			if (strncmp(buf, "quit", strlen("quit")) == 0 && end_flag == 1)
			{
				exit(1);
			}
			memset(buf, 0, BUF_SIZE);
		}
	}

	close(server_input);
	close(client_input);
	return 0;
}
