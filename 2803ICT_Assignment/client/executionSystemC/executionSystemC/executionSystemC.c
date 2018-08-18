/*
 ============================================================================
 Name        : executionSystemC.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "header.h"

int nonblock(SOCKET fd);
void closeConnection(SOCKET sock);
SOCKET connector(char *host);
int process(sock);

int main(int argc, char *argv[]) {
	setbuf(stdout, NULL);

	/* command line argument for server address */
	check(argc == 2, "USAGE: $>client 'netclient host'.\n");
	SOCKET sock = connector(argv[1]);

	/* main process */
	process(sock);

	/* clean up */
	closeConnection(sock);
	return EXIT_SUCCESS;
	error:
	return -1;
}

/* communication with server */
int process(int sock){
	int leaf = 1;

	/*  file descriptor */
	fd_set read_flags, write_flags;
	struct timeval waitd;

	/* timer keeper */
//	struct time_t cur;
//	unsigned int time_cost;

	/* usage */
	command_usage();

	/* polling wait I/O */
	int id = 0;
	while (leaf) {
		waitd.tv_sec = 10;
		FD_ZERO(&read_flags);
		FD_ZERO(&write_flags);
		FD_SET(sock, &read_flags);
		FD_SET(sock, &write_flags);

		/* select */
		switch(select(sock + 1, &read_flags, &write_flags, (fd_set*) 0, &waitd)){

		case -1: //select failed.
			perror("select failed.");
			exit(0);

		case 0: //keep polling
			break;

		default:
			/* socket ready for reading */
			if (FD_ISSET(sock, &read_flags)) {
				FD_CLR(sock, &read_flags);

				//Response response = malloc(sizeof(response));

				/*** Not finish yet **/
				/* read response */
//				check(read_response(sock, response) > 0, "Response failure");
				/** display time cost */
//				cur = time_t(NULL);
//				time_cost = (cur - response->current);
//				printf("SYSTEM: Request No. %d, cost time %d.\n", response->id, time_cost);

				/* display
				 * if argc > 40, for every 40 lines call getch(),
				 * stop displaying, waiting for user input to proceed.
				 * */
//				for(int i = 0; i< response->argc; i++){
//					if(i % 40 == 0)
//						getch();
//					printf("SYSTEM: Message from server: %s\n", response->comment);
//				}
			}

			//socket ready for writing
			if (FD_ISSET(sock, &write_flags)) {
				FD_CLR(sock, &write_flags);

				//Request request = malloc(sizeof(request));

				/** return 0 means 'quit' */
				//if(make_request(request) == 0){

					/* quit, break the loop */
					//leaf = false;
				//}else{
					/* send request */
					//request->id = ++id;
					//check(send_request(sock, request) > 0, "Error: send request");
				//}

			}
		}
	}

	/* return */
	return 0;
	error:
	closeConnection(sock);
	return -1;

}

/** client connector */
SOCKET connector(char *host) {
#ifdef WIN64
	WSADATA wsaData;
	check(WSAStartup(MAKEWORD(2, 2), &wsaData) == NO_ERROR, "[ERROR] WSAStartup failed");
#endif
	SOCKET connector;
	int sockOpt = 1;

	/* client address */
	SOCKADDR_IN addrClient;
	memset(&addrClient, 0, sizeof(addrClient));
	addrClient.sin_family = AF_INET;
	addrClient.sin_addr.s_addr = inet_addr(host);
	addrClient.sin_port = htons(DEFAULTPORT);

	/* get the socket*/
	connector = socket(AF_INET, SOCK_STREAM, 0);
	check(connector >= 0, "[ERROR] Cannot create a socket.");

	/* set 1. keep alive. 2. TCP no delay  */
	setsockopt(connector, SOL_SOCKET, SO_KEEPALIVE, &sockOpt, sizeof(unsigned int));
	setsockopt(connector, IPPROTO_TCP, TCP_NODELAY, &sockOpt, sizeof(unsigned int));

	/** connect */
	check(connect(connector, (SOCKADDR*) &addrClient, sizeof(addrClient)) == 0, "[ERROR] Cannot connect to server.");

	/** set nonblock */
	check(nonblock(connector) == 0, "Can't set nonblocking.");

	/* return */
	return connector;
	error:
	closeConnection(connector);
	return -1;
}

/* non blocking setting */
int nonblock(SOCKET fd) {
	int flags = fcntl(fd, F_GETFL, 0);
	return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

/* clean up socket */
void closeConnection(SOCKET sock){

#ifdef WIN64
	closesocket(sock);
	WSACleanup(); //Clean up Winsock
else
	close(sock);
#endif

}
