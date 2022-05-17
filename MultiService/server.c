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
#include <time.h>

#define PORTNUM 9000

int main(){
	char buf[BUFSIZ] = {0,};
	char service[200] = "[Service List]\n1. Get Current Time\n2. Download File\n3. Echo Server\nEnter:";
	char file[200] = "[Available File List]\n1. Book.txt\n2.HallymUniv.jpg\n3. Go back\nEnter:\n";
	struct sockaddr_in sin, cli;
	int access_socket, comm_socket;
	int clientlen = sizeof(cli);
	int serverlen = sizeof(sin);
	time_t rawtime;
	struct tm*timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	
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
		send(comm_socket,service,strlen(service)+1,0);

		memset(buf, '\0', BUFSIZ);
		recv(comm_socket,buf,sizeof(buf),0);
		if(atoi(buf)==1){
			printf("service %s",buf);
			send(comm_socket, asctime(timeinfo), strlen(asctime(timeinfo))+1, 0);
		}
		else if(atoi(buf)==2){
			printf("service %s",buf);
			memset(buf,'\0',BUFSIZ);
			send(comm_socket, file, strlen(file)+1, 0);
			memset(buf,'\0',BUFSIZ);
			recv(comm_socket, buf, sizeof(buf)+1,0);
			if(atoi(buf)==1){
				printf("file%s",buf);
			}
			else if(atoi(buf)==2){
				printf("file%s",buf);
			}
			else{
				printf("file%s",buf);
			}
		}
		else{
			printf("Termination of Service");
		}
	}
	close(access_socket);
	close(comm_socket);

	return 0;
}
