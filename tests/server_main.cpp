/*
 *	Author: Diego Ochando Torres
 *  Date: 16/03/2021
 *  e-mail: c0022981@my.shu.ac.uk | yeyoxando@gmail.com
 */

// ------------------------------------------------------------------------- //

#include <winsock2.h>
//#include <stdio.h>

#include "network_data.h"
#include "network_helpers.h"

// ------------------------------------------------------------------------- //

static int connected_clients = 0;
bool player_disconnected = false;
static SOCKET client_sock[2];

// ------------------------------------------------------------------------- //

static DWORD client_thread(void* client_socket) {
  
  SOCKET sock = (SOCKET)client_socket;
  int result = 0;
  int count = 0;
  int client_thread_id = -1;
	DataPackage data;
  std::vector<DataPackage> data_packages = std::vector<DataPackage>(0);


  lockMutex();
  count = ++connected_clients;
  unlockMutex();

  printf("\n Client thread started.");

	//Prepare client package to set player id
	data.package_kind = kDataPackageKind_Client;
	data.client.id = count;
	client_thread_id = count;
	//Send package
	send(sock, (char*)&data, sizeof(DataPackage), 0);

	//Added client
	printf("\nConnected client #%d.", connected_clients);
	
  while (true) {

    lockMutex();
    if (player_disconnected) break;
    unlockMutex();

    int receiving_cmd_count = 0;
	  // Receive first packet with num of commands to execute
		result = recv(sock, (char*)&data, sizeof(DataPackage), 0);
		if (result > 0) { // Something received
      if (data.header.cmd_count_ > 0) {
        receiving_cmd_count = data.header.cmd_count_;
        printf("\n NumCommands: %d", data.header.cmd_count_);
      }
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
			printf("\n Player %d data receive failed or player disconnected.", count);
			break;
		}

	  // Recv and process while not all commands have been received
    while (receiving_cmd_count > 0) {
			result = recv(sock, (char*)&data, sizeof(DataPackage), 0);
			if (result > 0) { // Something received
				// Get build command and send to the other player
        data_packages.push_back(data);
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
				printf("\n Player %d data receive failed or player disconnected.", count);
				break;
			}
      
      receiving_cmd_count--;
    }

		// Send first packet with number of commands
		// Send info about how many cmds have to receive the other player after this one (fixed step)
		data.package_kind = kDataPackageKind_Header;
		data.header.sender_id = client_thread_id;
		data.header.cmd_count_ = data_packages.size();
    int opponent_socket = count == 1 ? 1 : 0;
		send(client_sock[opponent_socket], (char*)&data, sizeof(DataPackage), 0);
		/*if (data.header.cmd_count_ > 0) {
			printf("\nSend data to the other player");
		}*/

	  // Send while not all commands have been sent
		// Send as many commands as have been accumulated in the cmd list (Variable step)
		while (data_packages.cbegin() != data_packages.cend()) {
			DataPackage data_to_opponent = data_packages[0];

			send(client_sock[opponent_socket], (char*)&data_to_opponent, sizeof(DataPackage), 0);

			data_packages.erase(data_packages.cbegin());
		}

    // OLD
		//Receive transform from player
		/*result = recv(sock, (char*)&data, sizeof(DataPackage), 0);
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
    */
	}

	int opponent_socket = count == 1 ? 1 : 0;
  send(client_sock[opponent_socket], 0, 0, 0);

  closesocket(sock);

  return 0;

}

// ------------------------------------------------------------------------- //

int main() {
  // Winsock variables
  WSADATA wsa;
  SOCKET sock;
  sockaddr_in server_ip;
  sockaddr_in clients_ip[2];
  int ip_length = sizeof(server_ip);
  int client_ip_length = sizeof(clients_ip[0]);
  int result = 0;

  //Server variables
  DataPackage data;

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