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
#include "network_manager.h"

// ------------------------------------------------------------------------- //

Transform p1;
Transform p2;
bool connection_running = false;

// ------------------------------------------------------------------------- //

static DWORD socket_thread(void* user_data) {

  connection_running = true;

	int result = 0;
  SOCKET sock;
	struct sockaddr_in server_ip;
	int ip_length = sizeof(server_ip);
	DataPackage data;
	Client client = { -1 };

	//Set socket attributes
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET) {
		WSACleanup();
		return EXIT_FAILURE;
	}
	server_ip = SetIP("127.0.0.1", 50000);

  printf("\n Socket thread started.");

	//Connect client to server ip
	result = connect(sock, (SOCKADDR*)&server_ip, ip_length);
	if (result == SOCKET_ERROR) {
		printf("\nERROR: client socket cannot bind.");
		closesocket(sock);
		WSACleanup();
		return EXIT_FAILURE;
	}
	else {
		printf("\nConnected.");
	}


  // Receive client id
	recv(sock, (char*)&data, sizeof(DataPackage), 0);
	if (data.package_kind == kDataPackageKind_Client) {
		client.id = data.client.id;
		printf("\nClient %d", client.id);
	}
	else {
		printf("\nERROR: Client data not received.");
		return 1;
	}



  while (true) {

    lockMutex();
    if (!connection_running) break;
    unlockMutex();

		//Send user current transform to server
		data.transform = client.id == 1 ? p1 : p2;
		send(sock, (char*)&data, sizeof(DataPackage), 0);

    // Receive data from the other user
		result = recv(sock, (char*)&data, sizeof(DataPackage), 0);
		if (result > 0) {
			if (client.id == 1) {
				p2 = data.transform;
			}
			else {
				p1 = data.transform;
			}
		}
		else{
			printf("\n Lost connection. Game finished.");
			lockMutex();
			connection_running = false;
			unlockMutex();
			break;
		}
  }

  send(sock, 0, 0, 0);

  closesocket(sock);

}

// ------------------------------------------------------------------------- //

int main(int argc, char* argv[]) {
  double current_time, last_time;
  unsigned char fps = 60;
  bool window_should_close = false;
  SDL_Event events;

  //Winsock variables
  WSADATA wsa;

  // Init window
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Window* window = SDL_CreateWindow("Network game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,600, 400, SDL_WINDOW_SHOWN);
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

  // TODO: Load sprites

  // TODO: Game things start

  // Winsock start
  if (WSAStartup(MAKEWORD(2, 0), &wsa) == 0) {
    printf("\nWinsock ready.");
  }
  else {
		printf("\nERROR: Winsock can't initialize.");
		return EXIT_FAILURE;
  }

  InitializeSRWLock(&mutex);

  // Network thread
  CreateThread(nullptr, 0, socket_thread, nullptr, 0, nullptr);

  // Game loop
  while (!window_should_close){ // Not works by closing it from the console

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
    /*data.transform = client.id == 1 ? p1 : p2;
		send(sock, (char*)&data, sizeof(DataPackage), 0);*/

		// Draw
		SDL_RenderClear(renderer);
		SDL_RenderPresent(renderer);
    

    // get frame time
    

    // end frame

    //Receive enemy server data
    /*result = recv(sock, (char*)&data, sizeof(DataPackage), 0);
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
    }*/

  }

  // Disconnect message
	//send(sock, 0, 0, 0);

  //Close sockets
  //closesocket(sock);
  //printf("\nSocket disconnected from server");

	lockMutex();
  connection_running = false;
  unlockMutex();
  // Wait for network processes to finish
  SDL_Delay(3000);

  //Shut down Winsock
  WSACleanup();

  // Destroy window
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();
  
  return EXIT_SUCCESS;

}