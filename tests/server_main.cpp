/*
 *	Author: Diego Ochando Torres
 *  Date: 16/03/2021
 *  e-mail: c0022981@my.shu.ac.uk | yeyoxando@gmail.com
 */

// ------------------------------------------------------------------------- //

#include <winsock2.h>
#include <stdio.h>

#include "game_data.h"

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
  int connected_clients = 0;
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

  //Wait for clients connection
  while (connected_clients < 2) {
    //Accept client
    client_sock[connected_clients] = accept(sock, (SOCKADDR*)&clients_ip[connected_clients], NULL);
		if (client_sock[connected_clients] == INVALID_SOCKET) {
			closesocket(sock);
			WSACleanup();
			return EXIT_FAILURE;
    }

    //Prepare client package to set player id
    data.package_kind = kDataPackageKind_Client;
    data.client.id = connected_clients + 1;
    
    //Send package
    send(client_sock[connected_clients], (char*)&data, sizeof(DataPackage), 0);
    
    //Added client
    connected_clients++;
    printf("\nConnected client #%d.", connected_clients);
  }

  //Game loop variables management
  while (1) {
    //Receive input player 1
    result = recv(client_sock[0], (char*)&data, sizeof(DataPackage), 0);
    if (result > 0) {
      printf("\n Data received p1: Transform. X: %d. Y: %d", data.transform.x, data.transform.y);
      transforms[0] = data.transform;
    }
    else if (result == 0) {
      printf("\n Player 1 lost connection.");
      send(client_sock[1], 0, 0, 0);
      connected_clients--;
      break;
    }
    else {
      printf("\n Player 1 data receive failed.");
    }
    //Receive input player 2
    result = recv(client_sock[1], (char*)&data, sizeof(DataPackage), 0);
    if (result > 0) {
      printf("\n Data received p2: Transform. X: %d. Y: %d", data.transform.x, data.transform.y);
      transforms[1] = data.transform;
		}
		else if (result == 0) {
			printf("\n Player 2 lost connection.");
			send(client_sock[0], 0, 0, 0);
			connected_clients--;
      break;
		}
		else {
			printf("\n Player 2 data receive failed.");
		}


    //Send transforms to both players
    data.package_kind = kDataPackageKind_Transform;
    //Player 2 position to player 1 client
    data.transform = transforms[1];
    send(client_sock[0], (char*)&data, sizeof(DataPackage), 0);
    //Player 1 position to player 2 client
    data.transform = transforms[0];
    send(client_sock[1], (char*)&data, sizeof(DataPackage), 0);

  }

  //Close sockets
  closesocket(sock);
  closesocket(client_sock[0]);
  closesocket(client_sock[1]);

  //Shut down Winsock
  WSACleanup();

  return EXIT_SUCCESS;

}