/*
 *	Author: Diego Ochando Torres
 *  Date: 16/03/2021
 *  e-mail: c0022981@my.shu.ac.uk | yeyoxando@gmail.com
 */

// ------------------------------------------------------------------------- //

#include <winsock2.h>
#include <stdio.h>

#include <SDL.h>
#include "game_data.h"

// ------------------------------------------------------------------------- //

int main(int argc, char* argv[]) {
  double current_time, last_time;
  unsigned char fps = 60;
  bool window_should_close = false;
  SDL_Event events;

  //Winsock variables
  int result = 0;
  WSADATA wsa;
  SOCKET sock;
  struct sockaddr_in server_ip;
  int ip_length = sizeof(server_ip);
  DataPackage data;
  Client client = { -1};
  
  // Game variables
  Transform p1;
  Transform p2;

  // Init window
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Window* window = SDL_CreateWindow("Network game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,600, 400, SDL_WINDOW_SHOWN);
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

  // TODO: Load sprites

  // Game things start
  p1.x = 0;
  p1.y = 0;
  p2.x = 100;
  p2.y = 100;

  // Winsock start
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
  server_ip = SetIP("127.0.0.1", 50000);

  //Connect client to server ip
  result = connect(sock, (SOCKADDR*)&server_ip, ip_length);
  if (result == SOCKET_ERROR) {
		printf("\nERROR: client socket cannot bind.");
		closesocket(sock);
		WSACleanup();
		return EXIT_FAILURE;
  }
  else{
    printf("\nConnected.");
  }

  //Receive client data
  recv(sock, (char*)&data, sizeof(DataPackage), 0);
  if (data.package_kind == kDataPackageKind_Client) {
    client.id = data.client.id;
    printf("\nClient %d", client.id);
  }
  else {
    printf("\nERROR: Client data not received.");
    return 1;
  }

  while (!window_should_close){

    // last time

    // Input
    SDL_PollEvent(&events);
    switch (events.type) {
    case SDL_QUIT: {
      window_should_close = true;
      break;
    }
    default: {
      break;
    }
    }

    // Update
    

		//Send user current transform to server
    data.transform = client.id == 1 ? p1 : p2;
		send(sock, (char*)&data, sizeof(DataPackage), 0);

		// Draw
		SDL_RenderClear(renderer);
		SDL_RenderPresent(renderer);
    

    // get frame time
    

    // end frame

    //Receive enemy server data
    result = recv(sock, (char*)&data, sizeof(DataPackage), 0);
    if (result > 0) {
      if (client.id == 1) {
        p2 = data.transform;
      }
      else {
        p1 = data.transform;
      }
    }
    else if (result == 0) {
			if (client.id == 1) {
        printf("\n Player 2 lost connection. Game finished.");
        window_should_close = true;
			}
			else {
				printf("\n Player 1 lost connection. Game finished.");
        window_should_close = true;
			}
    }
    else {
      printf("\n Data receive failed.");
    }

  }

  // Disconnect message
	send(sock, 0, 0, 0);

  //Close sockets
  closesocket(sock);
  printf("\nSocket disconnected from server");

  //Shut down Winsock
  WSACleanup();

  // TODO: Destroy window
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return EXIT_SUCCESS;

}