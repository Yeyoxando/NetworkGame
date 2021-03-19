/*
 *	Author: Diego Ochando Torres
 *  Date: 16/03/2021
 *  e-mail: c0022981@my.shu.ac.uk | yeyoxando@gmail.com
 */

// ------------------------------------------------------------------------- //

#include <winsock2.h>
#include <stdio.h>

#include "game_data.h"
#include "network_manager.h"

// ------------------------------------------------------------------------- //

static int connected_clients = 0;
bool player_disconnected = false;

// ------------------------------------------------------------------------- //

static DWORD client_thread(void* user_data) {
  
  SOCKET sock = (SOCKET)user_data;
  int result = 0;
  int count = 0;

	DataPackage data;
	Transform transforms[2];


  lockMutex();
  count = ++connected_clients;
  unlockMutex();

  printf("\n Client thread started.");

	//Prepare client package to set player id
	data.package_kind = kDataPackageKind_Client;
	data.client.id = count;
	//Send package
	send(sock, (char*)&data, sizeof(DataPackage), 0);

	//Added client
	printf("\nConnected client #%d.", connected_clients);

  while (connected_clients < 2) {}

	while (true) {

    lockMutex();
    if (player_disconnected) break;
    unlockMutex();

		//Receive transform from player
		result = recv(sock, (char*)&data, sizeof(DataPackage), 0);
		if (result > 0) {
			printf("\n Data received player %d: Transform. X: %d. Y: %d", count, data.transform.x, data.transform.y);
      lockMutex();
      transforms[count - 1] = data.transform;
      unlockMutex();
		}
		else if (result == 0) {
      lockMutex();
      count = --connected_clients;
      player_disconnected = true;
      unlockMutex();

			printf("\n A player has disconnected from the game.");
			break;
		}
		else {
			printf("\n Player %d data receive failed.", count);
		}


		//Send transform to other players
		data.package_kind = kDataPackageKind_Transform;
		//Player 2 position to player 1 client
    int other_player = count == 1 ? 1 : 0;

    lockMutex();
		data.transform = transforms[other_player];
    unlockMutex();

    send(sock, (char*)&data, sizeof(DataPackage), 0);

	}

  send(sock, 0, 0, 0);

  closesocket(sock);

  return 0;

}

// ------------------------------------------------------------------------- //

int main() {
  // Winsock variables
  WSADATA wsa;
  SOCKET sock;
  SOCKET client_sock[2];
  struct sockaddr_in server_ip;
  struct sockaddr_in clients_ip[2];
  int ip_length = sizeof(server_ip);
  int client_ip_length = sizeof(clients_ip[0]);
  int result = 0;

  //Server variables
  DataPackage data;
  Transform transforms[2];

  //Winsock start
  if (WSAStartup(MAKEWORD(2, 0), &wsa) == 0) {
    printf("\nWinsock ready.");
  }
  else {
    printf("\nERROR: Winsock can't initialize.");
    return EXIT_FAILURE;
  }


  //Set socket attributes
  sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sock == INVALID_SOCKET) {
    WSACleanup();
    return EXIT_FAILURE;
  }
  //Server ip setting 
  server_ip = SetIP("0.0.0.0", 50000);

  //Socket binding to server ip
  if (bind(sock, (SOCKADDR*)&server_ip, ip_length) == 0){
    printf("\nSocket binded correctly.");
  }
  else {
    printf("\nERROR: socket cannot bind.");
    closesocket(sock);
    WSACleanup();
    return EXIT_FAILURE;
  }

  //Enable socket listening to admit 2 connections
  if (listen(sock, 2) == 0){
    printf("\nSocket listening for 2 clients.");
  }
  else {
		printf("\nERROR: Socket not listening.");
		closesocket(sock);
		WSACleanup();
		return EXIT_FAILURE;
  }

  InitializeSRWLock(&mutex);

  //Wait for clients connection
  while (connected_clients < 2) {
    //Accept client
    client_sock[connected_clients] = accept(sock, (SOCKADDR*)&clients_ip[connected_clients], NULL);
		if (client_sock[connected_clients] == INVALID_SOCKET) {
			closesocket(sock);
			WSACleanup();
			return EXIT_FAILURE;
    }

    // Create a thread for the new client
    CreateThread(nullptr, 0, client_thread, (void*)client_sock[connected_clients], 0, nullptr);

  }

  // Wait while 2 clients are connected
  while (connected_clients == 2) {
    
  }

  // The console should wait a bit when closed until the rest of things get done

  //Close sockets
  closesocket(sock);

  //Shut down Winsock
  WSACleanup();

  return EXIT_SUCCESS;

}