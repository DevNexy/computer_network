#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <strings.h>

#define PORTNUM 9000

int main(void){
	char buf[BUFSIZ]={0,};
	struct sockaddr_in sin;
	int sock;
	int serverlen = sizeof(sin);

	if((sock=socket(AF_INET, SOCK_STREAM,0))==-1){
		exit(1);
	}
	memset((char*)&sin,'\0', serverlen);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORTNUM);
	sin.sin_addr.s_addr = inet_addr("127.0.0.1");

	if(connect(sock,(struct sockaddr *)&sin, serverlen)){
		exit(1);
	}
	while(1){
		memset(buf,'\0',BUFSIZ);
		recv(sock, buf, sizeof(buf), 0);

		if(!strcmp(buf,"quit\n"))
			break;
		printf("YOU: %s\n",buf);
		
		memset(buf,'\0',BUFSIZ);
		fgets(buf, BUFSIZ, stdin);

		if(!strcmp(buf,"quit\n"))
			break;
		send(sock,buf,strlen(buf)+1,0);
	}
	close(sock);
	return 0;
}
