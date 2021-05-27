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
	int executed_times = 0;

	DataPackage data;
	Client client = { -1 };

	//Set socket attributes
	if (initSocket(&sock) != 0) return EXIT_FAILURE;

	//Server ip setting 
	if (game->custom_ip_) {
		server_ip = SetIP(game->ip_, 50000); // User given IP		
	}
	else {
		server_ip = SetIP("127.0.0.1", 50000); // Local host
	}

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
		/*if (data.header.cmd_count_ > 0) {
			printf("\nFrame %d: Sending %d cmds.", executed_times, data.header.cmd_count_);
		}*/
		
		int send_cmd_count = data.header.cmd_count_;
		// Send as many commands as have been accumulated in the cmd list (Variable step)
		while (send_cmd_count > 0) {
			if (game->cmd_list_->commands_[0]->kind_ == kDataPackageKind_Build) {
				data.package_kind = kDataPackageKind_Build;
				data.build.sender_id = client.id;
				data.build.kind_ = (int)kDataPackageKind_Build; // Tower
				BuildData* build_data = static_cast<BuildData*>(game->cmd_list_->commands_[0]);
				data.build.x = build_data->x;
				data.build.y = build_data->y;
				data.build.build_kind = build_data->build_kind;
			}
			else if (game->cmd_list_->commands_[0]->kind_ == kDataPackageKind_Unit) {
				data.package_kind = kDataPackageKind_Unit;
				data.unit.sender_id = client.id;
				data.unit.kind_ = (int)kDataPackageKind_Unit;
				UnitData* unit_data = static_cast<UnitData*>(game->cmd_list_->commands_[0]);
				data.unit.x = unit_data->x;
				data.unit.y = unit_data->y;
				data.unit.id = unit_data->id;
				data.unit.active = unit_data->active;
				data.unit.hit_points = unit_data->hit_points;
			}
			else if (game->cmd_list_->commands_[0]->kind_ == (int)kDataPackageKind_UnitsEnd) {
				data.package_kind = kDataPackageKind_UnitsEnd;
				data.units_end.sender_id = client.id;
				data.units_end.kind_ = (int)kDataPackageKind_UnitsEnd;
				UnitsEnd* units_end_data = static_cast<UnitsEnd*>(game->cmd_list_->commands_[0]);
				data.units_end.end = units_end_data->end;
			}
			else if (game->cmd_list_->commands_[0]->kind_ == (int)kDataPackageKind_CastleLife) {
				data.package_kind = kDataPackageKind_CastleLife;
				data.castle_life.sender_id = client.id;
				data.castle_life.kind_ = (int)kDataPackageKind_CastleLife;
				CastleLife* castle_data = static_cast<CastleLife*>(game->cmd_list_->commands_[0]);
				data.castle_life.player_id = castle_data->player_id;
				data.castle_life.new_life = castle_data->new_life;
			}
			else if (game->cmd_list_->commands_[0]->kind_ == (int)kDataPackageKind_EndGame) {
				data.package_kind = kDataPackageKind_EndGame;
				data.end.sender_id = client.id;
				data.end.kind_ = (int)kDataPackageKind_EndGame;
				EndGame* end_game = static_cast<EndGame*>(game->cmd_list_->commands_[0]);
				data.end.winner_id = end_game->winner_id;
			}
			else if (game->cmd_list_->commands_[0]->kind_ == (int)kDataPackageKind_CaltropState) {
				data.package_kind = kDataPackageKind_CaltropState;
				data.caltrop.sender_id = client.id;
				data.caltrop.kind_ = (int)kDataPackageKind_CaltropState;
				CaltropState* caltrop = static_cast<CaltropState*>(game->cmd_list_->commands_[0]);
				data.caltrop.caltrop_id = caltrop->caltrop_id;
				data.caltrop.active = caltrop->active;
			}

			send(sock, (char*)&data, sizeof(DataPackage), 0);

			game->cmd_list_->commands_.erase(game->cmd_list_->commands_.cbegin());
			send_cmd_count--;
		}

		// Receive first packet with num of commands from the other player
		int receiving_cmd_count = 0;
		result = recv(sock, (char*)&data, sizeof(DataPackage), 0);
		if (result > 0) { // Something received
			if (data.header.cmd_count_ > 0) {
				receiving_cmd_count = data.header.cmd_count_;
				//printf("\n Frame %d: Recv %d cmds", executed_times, data.header.cmd_count_);
			}
		}
		else if (result == 0) {
			printf("\n Lost connection. Game finished.");
			lockMutex();
			connection_running = false;
			unlockMutex();
			break;
		}
		else {
			//printf("\nData receive failed cmd header.");
			game->disconnected_player_ = true;
		}

		// Recv and process while not all commands have been received
		while (receiving_cmd_count > 0) {
			result = recv(sock, (char*)&data, sizeof(DataPackage), 0);
			if (result > 0) { // Something received
				if (data.package_kind == kDataPackageKind_Build) {
					BuildData* build_data = CreateBuildData(data.build.sender_id, glm::vec2(data.build.x, data.build.y), data.build.build_kind);
					game->recv_cmd_list_->commands_.push_back(build_data);
				}
				else if (data.package_kind == kDataPackageKind_StartGame) {
					StartGame start_data;
					start_data.kind_ = (int)kDataPackageKind_StartGame;
					start_data.start = 1;
					game->recv_cmd_list_->commands_.push_back(&start_data);
				}
				else if (data.package_kind == kDataPackageKind_Unit) {
					UnitData* unit_data = CreateUnitData(data.unit.sender_id, glm::vec2(data.unit.x, data.unit.y),
						data.unit.id, data.unit.hit_points, data.unit.active);
					game->recv_cmd_list_->commands_.push_back(unit_data);
				}
				else if (data.package_kind == kDataPackageKind_UnitsEnd) {
					UnitsEnd* units_end = new UnitsEnd();
					units_end->sender_id = data.units_end.sender_id;
					units_end->kind_ = data.package_kind;
					units_end->end = data.units_end.end;
					game->recv_cmd_list_->commands_.push_back(units_end);
				}
				else if (data.package_kind == kDataPackageKind_CastleLife) {
					CastleLife* castle_life = new CastleLife();
					castle_life->kind_ = data.package_kind;
					castle_life->sender_id = data.castle_life.sender_id;
					castle_life->player_id = data.castle_life.player_id;
					castle_life->new_life = data.castle_life.new_life;
					game->recv_cmd_list_->commands_.push_back(castle_life);
				}
				else if (data.package_kind == kDataPackageKind_EndGame) {
					EndGame* end_game = new EndGame();
					end_game->kind_ = data.package_kind;
					end_game->sender_id = data.end.sender_id;
					end_game->winner_id = data.end.winner_id;
					game->recv_cmd_list_->commands_.push_back(end_game);
				}
				else if (data.package_kind == kDataPackageKind_CaltropState) {
					CaltropState* caltrop = new CaltropState();
					caltrop->kind_ = data.package_kind;
					caltrop->sender_id = data.caltrop.sender_id;
					caltrop->caltrop_id = data.caltrop.caltrop_id;
					caltrop->active = data.caltrop.active;
					game->recv_cmd_list_->commands_.push_back(caltrop);
				}
			}
			else if (result == 0) {
				printf("\n Lost connection. Game finished.");
				lockMutex();
				connection_running = false;
				unlockMutex();
				break;
			}
			else {
				//printf("\nData receive failed cmd list.");
				game->disconnected_player_ = true;
			}

			receiving_cmd_count--;
		}

		executed_times++;
		SuspendThread(hThread);

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
	if (argc > 1) {
		game->custom_ip_ = true;
		game->ip_ = argv[1];
		printf("\n Custom IP set: %s", argv[1]);
	}

	//Winsock start
	if (startWinsock(&wsa) != 0) return EXIT_FAILURE;

  InitializeSRWLock(&mutex);

  // Network thread
  hThread = CreateThread(nullptr, 0, socket_thread, (void*)game, 0, nullptr);
	Sleep(200);

	// Init internal engine things
	game->init();

	// Init resources and game
	game->loadGame();

  // Game loop
  while (!game->window_should_close_ || !connection_running){ // Not works by closing it from the console (Debug only)

    // last time
		frame_start = SDL_GetTicks();


    game->input();
    game->update(frame_delay);
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