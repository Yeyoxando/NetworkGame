/*
 *	Author: Diego Ochando Torres
 *  Date: 18/03/2021
 *  e-mail: c0022981@my.shu.ac.uk | yeyoxando@gmail.com
 */

 // ------------------------------------------------------------------------- //

#include <winsock2.h>
#include <stdio.h>

// ------------------------------------------------------------------------- //

#ifndef __NETWORK_HELPERS_H__
#define __NETWORK_HELPERS_H__

// ------------------------------------------------------------------------- //
// --- WINSOCK WRAPPERS ---

int initWinsock();

void closeWinsock();

//Returns a new ip settled with the given parameters
static sockaddr_in SetIP(const char* ip, int port) {
	sockaddr_in new_ip;

	new_ip.sin_family = AF_INET;
	new_ip.sin_addr.s_addr = inet_addr(ip);
	new_ip.sin_port = htons(port);

	return new_ip;
}

static int startWinsock(WSAData* data) {
	if (WSAStartup(MAKEWORD(2, 0), data) == 0) {
		printf("\nWinsock ready.");
	}
	else {
		printf("\nERROR: Winsock can't initialize.");
		return EXIT_FAILURE;
	}

	return 0;
}

static int initSocket(SOCKET* sock) {
	*sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (*sock == INVALID_SOCKET) {
		WSACleanup();
		return EXIT_FAILURE;
	}

	/*
	u_long iMode = 0;
	int iResult = ioctlsocket(*sock, FIONBIO, &iMode);
	if (iResult != NO_ERROR){
		printf("ioctlsocket failed with error: %ld\n", iResult);
	}
	*/

	return 0;
}

static int bindSocket(SOCKET* sock, SOCKADDR* server_ip, int ip_length) {
	if (bind(*sock, server_ip, ip_length) == 0) {
		printf("\nSocket binded correctly.");
	}
	else {
		printf("\nERROR: socket cannot bind.");
		closesocket(*sock);
		WSACleanup();
		return EXIT_FAILURE;
	}

	return 0;
}

static int connectSocket(SOCKET* sock, SOCKADDR* server_ip, int ip_length) {
	int result = connect(*sock, server_ip, ip_length);
	if (result == SOCKET_ERROR) {
		printf("\nERROR: client socket cannot bind. Server may not be running.");
		closesocket(*sock);
		WSACleanup();
		return EXIT_FAILURE;
	}
	else {
		printf("\nConnected.");
	}
	
	return 0;
}

static int listenConnections(SOCKET* sock, int connections) {
	if (listen(*sock, connections) == 0) {
		printf("\nSocket listening for 2 clients.");
	}
	else {
		printf("\nERROR: Socket not listening.");
		closesocket(*sock);
		WSACleanup();
		return EXIT_FAILURE;
	}

	return 0;
}

static int acceptClient(SOCKET* server_sock, SOCKET* client_sock, SOCKADDR* client_ip) {
	*client_sock = accept(*server_sock, client_ip, NULL);
	if (*client_sock == INVALID_SOCKET) {
		closesocket(*server_sock);
		WSACleanup();
		return EXIT_FAILURE;
	}

	return 0;
}

// ------------------------------------------------------------------------- //
// ------------------------------------------------------------------------- //
// ------------------------------------------------------------------------- //
// --- THREADS MUTEX ---

static SRWLOCK mutex;

static void lockMutex(void) {

	AcquireSRWLockExclusive(&mutex);

}

static void unlockMutex(void) {

	ReleaseSRWLockExclusive(&mutex);

}

// ------------------------------------------------------------------------- //
// ------------------------------------------------------------------------- //
// ------------------------------------------------------------------------- //
// --- SERVER AND CLIENT THREADS ---



// ------------------------------------------------------------------------- //

#endif // __NETWORK_HELPERS_H__
