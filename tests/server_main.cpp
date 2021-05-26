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
bool start_game_p1 = false;
bool start_game_p2 = false;
int recv_units_end = 0;
bool units_end_p1 = false;
bool units_end_p2 = false;
static SOCKET client_sock[2];

// ------------------------------------------------------------------------- //

static DWORD client_thread(void* client_socket) {
  
  SOCKET sock = (SOCKET)client_socket;
  int result = 0;
  int client_count = 0;
  int client_thread_id = -1;
  DataPackage data = {};
  std::vector<DataPackage> data_packages = std::vector<DataPackage>(0);


  lockMutex();
  client_count = ++connected_clients;
  unlockMutex();

  printf("\nClient thread started.");

	//Prepare client package to set player id
	data.package_kind = kDataPackageKind_Client;
	data.client.id = client_count;
	client_thread_id = client_count;
	//Send package
	send(sock, (char*)&data, sizeof(DataPackage), 0);

	//Added client
	printf("\nConnected client #%d.", connected_clients);
	
  while (true) {

    lockMutex();
    if (player_disconnected) {
      break;
    }
    unlockMutex();

    int receiving_cmd_count = 0;
	  // Receive first packet with num of commands to execute
		result = recv(sock, (char*)&data, sizeof(DataPackage), 0);
		if (result > 0) { // Something received
      if (data.header.cmd_count_ > 0) {
        receiving_cmd_count = data.header.cmd_count_;
        //printf("\n Player %d: Recv %d Commands", client_thread_id, data.header.cmd_count_);
      }
		}
    else if(result == 0) {
			lockMutex();
			client_count = --connected_clients;
			player_disconnected = true;
			unlockMutex();

			printf("\n A player has disconnected from the game.");
			break;
		}
		else {
			printf("\nData receive failed");
    } 

	  // Recv and process while not all commands have been received
    while (receiving_cmd_count > 0) {
			result = recv(sock, (char*)&data, sizeof(DataPackage), 0);
			if (result > 0) { // Something received
				// Get  command and send to the other player
        if (data.package_kind == kDataPackageKind_UnitsEnd) {
          lockMutex();
          recv_units_end++;
          unlockMutex();
        }
        else {
          data_packages.push_back(data);
        }
			}
      else if (result == 0) {
				lockMutex();
				client_count = --connected_clients;
				player_disconnected = true;
				unlockMutex();

				printf("\n A player has disconnected from the game.");
				break;
			}
			else {
				printf("\nData receive failed");
			}
      
      receiving_cmd_count--;
    }

    lockMutex();
    // GAME START
    if (start_game_p1 && client_thread_id == 1) {
      start_game_p1 = false;
      data.package_kind = kDataPackageKind_StartGame;
      data.start.start = 1;
      data_packages.push_back(data);
    }

		if (start_game_p2 && client_thread_id == 2) {
			start_game_p2 = false;
			data.package_kind = kDataPackageKind_StartGame;
			data.start.start = 1;
			data_packages.push_back(data);
		}

    // UNITS END
    if (recv_units_end == 2) {
      recv_units_end = 0;
      units_end_p1 = true;
      units_end_p2 = true;
    }
    if (units_end_p1 && client_thread_id == 1) {
      units_end_p1 = false;    
      data.package_kind = kDataPackageKind_UnitsEnd;
      data.units_end.end = 1;
      data_packages.push_back(data);
    }
		if (units_end_p2 && client_thread_id == 2) {
      units_end_p2 = false;    
      data.package_kind = kDataPackageKind_UnitsEnd;
      data.units_end.end = 1;
      data_packages.push_back(data);
		}
    unlockMutex();

		// Send first packet with number of commands
		// Send info about how many cmds have to receive the other player after this one (fixed step)
		data.package_kind = kDataPackageKind_Header;
		data.header.sender_id = client_thread_id;
		data.header.cmd_count_ = data_packages.size();
    int opponent_socket = client_count == 1 ? 1 : 0;
		send(client_sock[opponent_socket], (char*)&data, sizeof(DataPackage), 0);
		/*if (data.header.cmd_count_ > 0) {
			printf("\nPlayer %d send %d data to the other player", client_thread_id, data.header.cmd_count_);
		}*/

	  // Send while not all commands have been sent
		// Send as many commands as have been accumulated in the cmd list (Variable step)
		while (data_packages.cbegin() != data_packages.cend()) {
			DataPackage data_to_opponent = data_packages[0];

			send(client_sock[opponent_socket], (char*)&data_to_opponent, sizeof(DataPackage), 0);

			data_packages.erase(data_packages.cbegin());
		}
 	}
	

	int opponent_socket = client_count == 1 ? 1 : 0;
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

  lockMutex();
  start_game_p1 = true;
  start_game_p2 = true;
	units_end_p1 = false;
	units_end_p2 = false;
  unlockMutex();
  printf("\nGame started.");

  // Wait while 2 clients are connected
  while (internal_clients == 2) {
    if (connected_clients < 2) break;

    // Server updates
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