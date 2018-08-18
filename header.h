/*
 * header.h
 *
 *  Created on: 17 Aug. 2018
 *      Author: Firklaag_ins
 */

#ifndef HEADER_H_
#define HEADER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifdef WIN64
#include <winsock2.h>
#include <windows.h>
#pragma comment(lib,"Ws2_32.lib")
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#endif

#define DEFAULTPORT 80
#define BUFFERSIZE 1024

#ifndef WIN64
typedef unsigned int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
#endif

#define check(A, M, ...) if(!(A)) { perror(M); errno=0; goto error;}

#endif /* HEADER_H_ */
