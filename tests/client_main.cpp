/*
 *	Author: Diego Ochando Torres
 *  Date: 16/03/2021
 *  e-mail: c0022981@my.shu.ac.uk | yeyoxando@gmail.com
 */

// ------------------------------------------------------------------------- //

#include <winsock2.h>
#include <stdio.h>

#include <SDL.h>
#include "network_game.h"
#include "network_data.h"
#include "network_helpers.h"

// ------------------------------------------------------------------------- //

bool connection_running = true;
HANDLE hThread;

// ------------------------------------------------------------------------- //

static DWORD socket_thread(void* game_data) {

  connection_running = true;

	NetworkGame* game = static_cast<NetworkGame*>(game_data);

	int result = 0;
  SOCKET sock;
	sockaddr_in server_ip;
	int ip_length = sizeof(server_ip);

	DataPackage data;
	Client client = { -1 };

	//Set socket attributes
	if (initSocket(&sock) != 0) return EXIT_FAILURE;

	//Server ip setting 
	server_ip = SetIP("127.0.0.1", 50000);

	//Connect client to server ip
	if (connectSocket(&sock, (SOCKADDR*)&server_ip, ip_length) != 0) return EXIT_FAILURE;

  printf("\n Socket thread started.");

  // Receive client id
	recv(sock, (char*)&data, sizeof(DataPackage), 0);
	if (data.package_kind == kDataPackageKind_Client) {
		client.id = data.client.id;
		game->client_id_ = client.id;
		printf("\nClient %d", client.id);
	}
	else {
		printf("\nERROR: Client data not received.");
		return 1;
	}


  while (true) {

		// ResumeThread is called after the main thread update function, so the net thread can run
		//   in parallel while the update function is not modifying the command list (During render, FPS limit and input from the main thread)

    lockMutex();
    if (!connection_running) break;
    unlockMutex();

		// Send info about how many cmds have to receive the server after this one (fixed step)
		data.package_kind = kDataPackageKind_Header;
		data.header.sender_id = client.id;
		data.header.cmd_count_ = game->cmd_list_->commands_.size();
		send(sock, (char*)&data, sizeof(DataPackage), 0);
		if (data.header.cmd_count_ > 0) {
			printf("\nSend");
		}

		// Send as many commands as have been accumulated in the cmd list (Variable step)
		while (game->cmd_list_->commands_.cbegin() != game->cmd_list_->commands_.cend()) {
			data.package_kind = kDataPackageKind_Build;
			data.build.sender_id = client.id;
			BuildData* build_data = static_cast<BuildData*>(game->cmd_list_->commands_[0]);
			data.build.kind_ = (int)build_data->kind_; // Tower
			data.build.x = build_data->x;
			data.build.y = build_data->y;

			send(sock, (char*)&data, sizeof(DataPackage), 0);

			game->cmd_list_->commands_.erase(game->cmd_list_->commands_.cbegin());
		}

		// Receive first packet with num of commands from the other player
		int receiving_cmd_count = 0;
		result = recv(sock, (char*)&data, sizeof(DataPackage), 0);
		if (result > 0) { // Something received
			if (data.header.cmd_count_ > 0) {
				receiving_cmd_count = data.header.cmd_count_;
				printf("\n NumCommands: %d", data.header.cmd_count_);
			}
		}
		else if (result == 0) {
			printf("\n Lost connection. Game finished.");
			lockMutex();
			connection_running = false;
			unlockMutex();
			break;
		}

		// Recv and process while not all commands have been received
		while (receiving_cmd_count > 0) {
			result = recv(sock, (char*)&data, sizeof(DataPackage), 0);
			if (result > 0) { // Something received
				BuildData* build_data = CreateBuildData(data.build.sender_id, glm::vec2(data.build.x, data.build.y));
				game->recv_cmd_list_->commands_.push_back(build_data);
			}
			else if (result == 0) {
				printf("\n Lost connection. Game finished.");
				lockMutex();
				connection_running = false;
				unlockMutex();
				break;
			}

			receiving_cmd_count--;
		}


		SuspendThread(hThread);

		// OLD
		//Send user current transform to server
		/*data.transform = client.id == 1 ? game->p1 : game->p2;
		send(sock, (char*)&data, sizeof(DataPackage), 0);

    // Receive data from the other user
		result = recv(sock, (char*)&data, sizeof(DataPackage), 0);
		if (result > 0) {
			if (client.id == 1) {
				game->p2 = data.transform;
			}
			else {
				game->p1 = data.transform;
			}
		}
		else if(result == 0){
			printf("\n Lost connection. Game finished.");
			lockMutex();
			connection_running = false;
			unlockMutex();
			break;
		}*/
  }

	// Send disconnection message
  send(sock, 0, 0, 0);

  closesocket(sock);

}

// ------------------------------------------------------------------------- //

int main(int argc, char* argv[]) {
	const int fps = 60;
	const int frame_delay = 1000 / fps;
	uint32_t frame_start;
	int frame_time;

	//Winsock variables
	WSADATA wsa;

  // Game
  NetworkGame* game = &NetworkGame::instance();


	//Winsock start
	if (startWinsock(&wsa) != 0) return EXIT_FAILURE;

  InitializeSRWLock(&mutex);

  // Network thread
  hThread = CreateThread(nullptr, 0, socket_thread, (void*)game, 0, nullptr);
	Sleep(200);

	// Init internal engine things
	game->init();

	// Init resources and game
	game->loadResources();
	game->loadGame();

  // Game loop
  while (!game->window_should_close_ || !connection_running){ // Not works by closing it from the console (Debug only)

    // last time
		frame_start = SDL_GetTicks();


    game->input();
    game->update();
		ResumeThread(hThread);
    game->draw();
    

    // get frame time
		frame_time = SDL_GetTicks() - frame_start;
		// limit
		if (frame_delay > frame_time) {
			SDL_Delay(frame_delay - frame_time);
		}

  }

	lockMutex();
  connection_running = false;
  unlockMutex();
  
  // Wait for network processes to finish
  SDL_Delay(1000);

  //Shut down Winsock
  WSACleanup();

  // Destroy game resources
  game->close();
	delete game;

  return EXIT_SUCCESS;

}