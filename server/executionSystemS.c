/*
 ============================================================================
 Name        : executionSystemS.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "header.h"

/* define functions */
SOCKET connector();
int listener(SOCKET sock);
void closeConnection(SOCKET socket);
void sigChld(int signo);

/* main entry */
int main(void) {
	setbuf(stdout, NULL);

	SOCKET sock = connector();

	listener(sock);

	return EXIT_SUCCESS;
}

/* get socket connection */
SOCKET connector(){

#ifdef WIN64
	WSADATA wsaData;
	check(WSAStartup(MAKEWORD(2, 2), &wsaData) == NO_ERROR, "[ERROR] WSAStartup failed");
#endif

	SOCKET listenFd;
	int sockOpt = 1;

	SOCKADDR_IN addrServer;
	memset(&addrServer, 0, sizeof(addrServer));
	addrServer.sin_family = AF_INET;
	addrServer.sin_addr.s_addr = htonl(INADDR_ANY);
	addrServer.sin_port = htons(DEFAULTPORT);

	listenFd = socket(AF_INET, SOCK_STREAM, 0);
	check(listenFd > 0, "[Error] Initialize socket failed");

	printf("[INFO] Socket created: %d\n", listenFd);

	setsockopt(listenFd, SOL_SOCKET, SO_KEEPALIVE, &sockOpt, sizeof(unsigned int));
	setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR, &sockOpt, sizeof(unsigned int));
	setsockopt(listenFd, IPPROTO_TCP, TCP_NODELAY, &sockOpt, sizeof(unsigned int));

	check(bind(listenFd, (SOCKADDR *) & addrServer, sizeof (addrServer)) == 0, "[Error] Binding failed");

	printf("[INFO] Socket bind on port: %d\n", DEFAULTPORT);

	check(listen(listenFd, SOMAXCONN) == 0, "[ERROR] Listening failed");

	printf("[INFO] Socket start listening.\n");

	return listenFd;

	error:
	closeConnection(listenFd);
	return -1;
}

/* Handling coming clients */
int listener(SOCKET sock){
	/* process id */
	pid_t pid;

	SOCKADDR_IN addrClient;
	int len = sizeof(SOCKADDR);

	/* kill the zombie */
	signal(SIGCHLD , sigChld);

	/** wait for client */
	while(1){
		/* accept for client */
		SOCKET sockfd_ptr = accept(sock, (SOCKADDR*) &addrClient, &len);
		check(sockfd_ptr > 0, "Error: accept");

		printf("[INFO] New connection.	\n");

		/* create fork */
		pid = fork();
		check(pid != -1, "fork error");

		/* child process */
		if(pid == 0){
			/* deal with client asking */
			reception(sockfd_ptr);
			free(sockfd_ptr);
			return 0; /* return end child process for good */
		}else{//parents process
			free(sockfd_ptr);
		}
	}

	return 0;
	error:
	//free(sockfd_ptr);
	closeConnection(sock);
	return -1;
}

/* sig function for prevent zombie process*/
void sig_chld(int signo){
	pid_t pid;
	int stat;
	while((pid = waitpid(-1, &stat, WNOHANG)) > 0)
		printf("[INFO] child %d of client terminated.\n", (int)pid);
	return;
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
