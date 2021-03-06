#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define PORTNUM 9001

int main(void){
	char buf[256];
	struct sockaddr_in sin,cli;
	int sd, ns, clientlen = sizeof(cli);
	int client_cnt =0;

	if((sd=socket(AF_INET, SOCK_STREAM, 0))==-1){
		perror("socket");
		exit(1);
	}

	memset((char *)&sin, '\0',sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORTNUM);
	sin.sin_addr.s_addr = inet_addr("127.0.0.1");

	if(bind(sd, (struct sockaddr *)&sin, sizeof(sin))){
		perror("bind");
		exit(1);
	}
	if(listen(sd, 5)){
		perror("listen");
		exit(1);
	}
	while(1){
		if((ns = accept(sd, (struct sockaddr*)&cli, &clientlen))==-1){
			perror("accept");
			exit(1);
		}
		client_cnt +=1;
		printf("New client!\nNumber of service client: %d\n", client_cnt);
		switch(fork()){
			case 0:
				close(sd);
				while(1){
					if(recv(ns, buf, sizeof(buf),0)== -1){
						perror("recv");
						exit(1);
					}
					printf("Recv from Client: %s\n",buf);
					if(send(ns, buf, strlen(buf)+1, 0)== -1){
						perror("send");
						exit(1);
					}
				}
				exit(0);
		}
		// close(ns);
	}
	//close(sd);
	close(ns);
	return 0;
}
