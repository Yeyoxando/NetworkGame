/*
 *	Author: Diego Ochando Torres
 *  Date: 16/03/2021
 *  e-mail: c0022981@my.shu.ac.uk | yeyoxando@gmail.com
 */

// ------------------------------------------------------------------------- //

#include <winsock2.h>
#include <stdio.h>

#include "network_data.h"
#include "network_helpers.h"

// ------------------------------------------------------------------------- //

static int connected_clients = 0;
bool player_disconnected = false;

// ------------------------------------------------------------------------- //

static DWORD client_thread(void* user_data) {
  
  SOCKET sock = (SOCKET)user_data;
  int result = 0;
  int count = 0;

	DataPackage data;
	TransformTest transforms[2];


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
	
  while (true) {

    lockMutex();
    if (player_disconnected) break;
    unlockMutex();

		//Receive transform from player
		result = recv(sock, (char*)&data, sizeof(DataPackage), 0);
		if (result > 0) {
			//printf("\n Data received player %d: Transform. X: %d. Y: %d", count, data.transform.x, data.transform.y);
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
		//Player 2 position to player 1 client and vice versa
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
  sockaddr_in server_ip;
  sockaddr_in clients_ip[2];
  int ip_length = sizeof(server_ip);
  int client_ip_length = sizeof(clients_ip[0]);
  int result = 0;

  //Server variables
  DataPackage data;
  TransformTest transforms[2];

	//Winsock start
  if (startWinsock(&wsa) != 0) return EXIT_FAILURE;

  //Set socket attributes
  if (initSocket(&sock) != 0) return EXIT_FAILURE;
  
  //Server ip setting 
  server_ip = SetIP("0.0.0.0", 50000);

  //Socket binding to server ip
  if (bindSocket(&sock, (SOCKADDR*)&server_ip, ip_length) != 0) return EXIT_FAILURE;

  //Enable socket listening to admit 2 connections
  if (listenConnections(&sock, 2) != 0) return EXIT_FAILURE;

  // Init mutex
  InitializeSRWLock(&mutex);

  int internal_clients = 0;
  //Wait for clients connection
  while (internal_clients < 2) {
    //Accept client
    if (acceptClient(&sock, &client_sock[connected_clients], (SOCKADDR*)&clients_ip[connected_clients]) != 0) return EXIT_FAILURE;
    ++internal_clients;

    // Create a thread for the new client
		CreateThread(nullptr, 0, client_thread, (void*)client_sock[connected_clients], 0, nullptr);
		Sleep(1000);
  }

  // Wait while 2 clients are connected
  while (internal_clients == 2) {
    if (connected_clients < 2) break;
  }

  // The console should wait a bit when closed until the rest of things get done
  if (client_sock[0] != INVALID_SOCKET)
    send(client_sock[0], 0, 0, 0);

	if (client_sock[1] != INVALID_SOCKET)
		send(client_sock[1], 0, 0, 0);

  //Close sockets
  closesocket(sock);

  //Shut down Winsock
  WSACleanup();

  return EXIT_SUCCESS;

}