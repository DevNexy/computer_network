#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void handler(int sig){
	printf("Handler is called.\n");
	exit(EXIT_SUCCESS);
}
int main(){
	signal(SIGINT, handler);
	printf("Sleep begins!\n");
	sleep(1000);
	printf("Wake up!\n");

	return 0;
}
