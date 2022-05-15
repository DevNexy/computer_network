#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <strings.h>

#define BUF_SIZE 100
#define SOCK_ADDR "./sock_addr"
int main()
{
	int server, client, len;
	struct sockaddr_un server_addr;
	struct sockaddr client_addr;
	char buf[BUF_SIZE] ={0, };

	server_addr.sun_family = AF_UNIX;
	strcpy(server_addr.sun_path, SOCK_ADDR);
	
	server =socket(AF_UNIX, SOCK_STREAM, 0);
	if(server == -1)
	{
		perror("Server Error\n");
		exit(1);
	}


	if(bind(server, (struct sockaddr *)&server_addr, sizeof(server_addr))==-1){
		perror("Bind Error\n");
		exit(1);
	}
	if(listen(server,5)==-1){
		perror("listen Eror\n");
		exit(1);
	}
	client = accept(server, &client_addr, &len);

	while(1){
		if(recv(client,buf,BUF_SIZE+1,0)> 0){
			printf("Server recv message : %s\n", buf);
			memset(buf, 1, BUF_SIZE);
		}	

		if(strncmp(buf, "quit", strlen("quit"))==0)
		{
			break;
		}
	}
	close(server);
	close(client);

	return 0;
}
