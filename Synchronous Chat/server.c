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
#include <arpa/inet.h>

#define PORTNUM 9000

int main(){
	char buf[BUFSIZ] = {0,};
	struct sockaddr_in sin, cli;
	int access_socket, comm_socket;
	int clientlen = sizeof(cli);
	int serverlen = sizeof(sin);

	if((access_socket = socket(AF_INET, SOCK_STREAM,0))==-1){
		exit(1);
	}
	memset((char*)&sin, '\0',serverlen);
	sin.sin_family =AF_INET;
	sin.sin_port = htons(PORTNUM);
	sin.sin_addr.s_addr = inet_addr("127.0.0.1");

	if(bind(access_socket,(struct sockaddr *)&sin,serverlen)){
		exit(1);
	}
	if(listen(access_socket, 5)){
		exit(1);
	}
	if((comm_socket = accept(access_socket,(struct sockaddr *)&cli,&clientlen))==-1){
		exit(1);
	}
	while(1){
		memset(buf,'\0',BUFSIZ);
		fgets(buf,BUFSIZ, stdin);

		if(!strcmp(buf,"quit"))
			break;
		
		send(comm_socket, buf, strlen(buf)+1, 0);
		memset(buf, '\0', BUFSIZ);
		recv(comm_socket,buf,sizeof(buf),0);

		if(!strcmp(buf, "quit"))
			break;
		printf("YOU:%s\n",buf);
	}
	close(access_socket);
	close(comm_socket);

	return 0;
}
