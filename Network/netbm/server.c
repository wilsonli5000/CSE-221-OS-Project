/************
 * server.c
 * accepting packet and ack
 ***************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include "netbm.h"

void error(const char *msg)
{
	fprintf(stderr, "%s", msg);
	exit(0);
}

int main(void)
{
	struct sockaddr_in servaddr, cliaddr;
	socklen_t cliaddr_len;
	int listenfd, connfd;
	char buf[MAXLINE];
	char str[INET_ADDRSTRLEN];
	int i, n, recvd;
	cpu_cycle ts1, ts2;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd < 0)
		error("ERROR opening socket\n");

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

	if (bind(listenfd, (struct sockaddr *)&servaddr, 
				sizeof(servaddr)) < 0)
		error("ERROR binding\n");

	// Start listen, allow 20 backlogs
	listen(listenfd, 20);

	printf("Accepting connections...\n");
	while (1) {
		cliaddr_len = sizeof(cliaddr);
		connfd = accept(listenfd, (struct sockaddr *)&cliaddr, 
				&cliaddr_len);
		if (connfd < 0)
			error("Error on accept\n");

		//receive part for RTT
		while ((n = read(connfd, buf, 255)) > 0){
    		printf("Message received!\n");
    		inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str));
			printf("received %d bytes from %s at PORT %d\n", n, str, 
				ntohs(cliaddr.sin_port));

    		int n1 = write(connfd,"1234567891234567891234567891234567891234567891234567891234567891",64);
    		if (n1 < 0) error("ERROR writing to socket");
    	}
		// Server may need more times to recv big packets

    	//server part for peak bandwidth
    		/*
		recvd = 0;
		while (1) {
			n = read(connfd, buf, MAXLINE);
			if (n == 0) {
				printf("the client has been closed\n");
				break;
			}
			recvd += n;
			if (recvd == DATASIZE*TRANSTIMES)
				break;
			
			inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str));
			printf("received %d bytes from %s at PORT %d\n", n, str, 
				ntohs(cliaddr.sin_port));
			printf("%s\n", buf);	
		}
		write(connfd, buf, DATASIZE);
		*/
		/*
		printf("transmit complete\n");
		ts1 = rdtsc();
		close(connfd);
		ts2 = rdtsc();
		printf("close time = %llu\n", ts2-ts1);
		break;
		*/
	}

	close(listenfd);
	return 0;
}
