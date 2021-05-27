/*
 *	Author: Diego Ochando Torres
 *  Date: 19/03/2021
 *  e-mail: c0022981@my.shu.ac.uk | yeyoxando@gmail.com
 */

// ------------------------------------------------------------------------- //

#include "network_game.h"
#include "network_game_maps.h"

// ------------------------------------------------------------------------- //

NetworkGame::NetworkGame(){

	window_ = nullptr;
	renderer_ = nullptr;
	tex_loader_ = nullptr;
	cmd_list_ = new CommandList();
	recv_cmd_list_ = new CommandList();

	window_should_close_ = false;

	client_id_ = 0;

	custom_ip_ = false;
	ip_ = "\0";

}

// ------------------------------------------------------------------------- //

NetworkGame::~NetworkGame() {

	delete cmd_list_;
	delete recv_cmd_list_;

}

// ------------------------------------------------------------------------- //

NetworkGame& NetworkGame::instance(){
	
	static NetworkGame* instance = new NetworkGame();
	return *instance;

}

// ------------------------------------------------------------------------- //

void NetworkGame::init() {

	SDL_Init(SDL_INIT_EVERYTHING);
	

	window_ = SDL_CreateWindow("Age of Towers", SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, 608, 640, SDL_WINDOW_SHOWN);
	

	renderer_ = SDL_CreateRenderer(window_, -1, 0);
	SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);


	tex_loader_ = new TextureLoader();

	scene_ = new Scene();
	scene_->init();

}

// ------------------------------------------------------------------------- //

void NetworkGame::loadGame() {

	// Create game things
	build_manager_ = new BuildManager();
	build_manager_->init(client_id_);
	unit_manager_ = new UnitManager();
	unit_manager_->init(client_id_);

	game_menus_ = new GameMenus();
	game_menus_->initGUI();

	game_started_ = false;

	castle_life_p1_ = 10;
	castle_life_p2_ = 10;

	game_end_ = false;
	winner_ = false;
	loser_ = false;

	disconnected_player_ = false;

}

// ------------------------------------------------------------------------- //

void NetworkGame::input() {

	SDL_PollEvent(&events_);

	SDL_GetMouseState(&mouse_pos_x_, &mouse_pos_y_);

	switch (events_.type) {
	case SDL_QUIT: {
		window_should_close_ = true;
		break;
	}
	case SDL_KEYDOWN: {
		/* Check the SDLKey values and move change the coords */
		switch (events_.key.keysym.sym) {
		/*case SDLK_b: {
			game_menus_->build_mode_ = !game_menus_->build_mode_;
			if (game_menus_->build_mode_) {
				build_manager_->mouse_build_object_->active_ = true;
			}
			else {
				build_manager_->mouse_build_object_->active_ = false;
			}
			break;
		}*/
		default: {
			break;
		}
		}
		break;
	}
	case SDL_MOUSEBUTTONDOWN: {
		if (events_.button.button == SDL_BUTTON_LEFT) {
			if (game_menus_->build_mode_ && mouse_pos_y_ <= 400) {
				build_manager_->createBuilding(true, transformed_mouse_x_, transformed_mouse_y_,
					client_id_, (int)build_manager_->selected_build_);
			}
		}
		break;
	}
	default: {
		break;
	}
	}

	game_menus_->inputGUI(events_);

}

// ------------------------------------------------------------------------- //

void NetworkGame::update(uint32_t time_step) {

	game_menus_->manageGUI();

	if (game_menus_->build_mode_) {
		// Fix position with map tiling
		transformed_mouse_x_ = mouse_pos_x_ - (mouse_pos_x_ % 16) + 8.0f;
		transformed_mouse_y_ = mouse_pos_y_ - (mouse_pos_y_ % 16);

		if (build_manager_->selected_build_ == kBuildKind_DefenseTower){
			build_manager_->mouse_build_object_->transform_.position_ = glm::vec3(transformed_mouse_x_,
				transformed_mouse_y_, 0.0f);
		} 
		else{
			build_manager_->mouse_build_object_->transform_.position_ = glm::vec3(transformed_mouse_x_,
				transformed_mouse_y_ + 8.0f, 0.0f);
		}
	}

	unit_manager_->update(time_step);

	NetworkGame::instance().scene_->update(time_step);

	// Execute other player received commands
	processNetCommands();
	
}

// ------------------------------------------------------------------------- //

void NetworkGame::draw() {
	
	// Draw things
	NetworkGame::instance().scene_->draw();
	unit_manager_->drawPaths();

	game_menus_->drawGUI();


	SDL_RenderPresent(renderer_);
	SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
	SDL_RenderClear(renderer_);

}

// ------------------------------------------------------------------------- //

void NetworkGame::close() {

	delete build_manager_;

	game_menus_->shutdownGUI();
	delete game_menus_;

	scene_->finish();
	delete scene_;

	delete tex_loader_;

	SDL_DestroyRenderer(renderer_);
	SDL_DestroyWindow(window_);

	SDL_Quit();

}

// ------------------------------------------------------------------------- //

Scene* NetworkGame::getScene(){

	return scene_;

}

// ------------------------------------------------------------------------- //

void NetworkGame::updateCastleLife(bool send_command, int player_id, int castle_life){

	// if life is 0 send the winner cmd too

	if (send_command) {
		CastleLife* castle_life_cmd = new CastleLife();
		castle_life_cmd->sender_id = client_id_;
		castle_life_cmd->kind_ = (int)kDataPackageKind_CastleLife;
		if (player_id == 2) {
			castle_life_p2_ = castle_life;			
			castle_life_cmd->player_id = 2;
			castle_life_cmd->new_life = castle_life_p2_;
		}
		else {
			castle_life_p1_ = castle_life;			
			castle_life_cmd->player_id = 1;
			castle_life_cmd->new_life = castle_life_p1_;
		}
		NetworkGame::instance().cmd_list_->commands_.push_back(castle_life_cmd);

		updateGameWinCondition(true, client_id_);
	}
	else {
		if (player_id == 2) {
			castle_life_p2_ = castle_life;			
		}
		else {
			castle_life_p1_ = castle_life;
		}
	}

}

// ------------------------------------------------------------------------- //

void NetworkGame::updateGameWinCondition(bool send_command, int winner_id){

	if (send_command) {
		// the winner player sends the end game cmd
		if (winner_id == 2) {
			if (castle_life_p1_ == 0) {
				EndGame* end_game = new EndGame();
				end_game->kind_ = (int)kDataPackageKind_EndGame;
				end_game->sender_id = winner_id;
				end_game->winner_id = winner_id;
				NetworkGame::instance().cmd_list_->commands_.push_back(end_game);

				game_end_ = true;
				winner_ = true;
				printf("\nPlayer 2 wins");
			}
		}
		else {
			if (castle_life_p2_ == 0) {
				EndGame* end_game = new EndGame();
				end_game->kind_ = (int)kDataPackageKind_EndGame;
				end_game->sender_id = winner_id;
				end_game->winner_id = winner_id;
				NetworkGame::instance().cmd_list_->commands_.push_back(end_game);

				game_end_ = true;
				winner_ = true;
				printf("\nPlayer 1 wins");
			}
		}
	}
	else {
		// Recv, set lose active
		game_end_ = true;
		loser_ = true;
		printf("\nPlayer lose.");
	}
	

}

// ------------------------------------------------------------------------- //

void NetworkGame::processNetCommands(){

	CommandList* cmd_list = new CommandList();
	cmd_list->commands_ = recv_cmd_list_->commands_;
	recv_cmd_list_->commands_.clear();

	while (cmd_list->commands_.cbegin() != cmd_list->commands_.cend()) {
		switch (cmd_list->commands_[0]->kind_){
		case kDataPackageKind_StartGame: {
			game_started_ = true;
			unit_manager_->reactivateUnits(client_id_, true);
			break;
		}
		case kDataPackageKind_UnitsEnd: {
			//printf("\nProcess Unit end player %d", client_id_);
			// add resources
			build_manager_->updateBuildings(client_id_);
			// Reactivate units
			unit_manager_->reactivateUnits(client_id_, true);
			break;
		}
		case kDataPackageKind_Build: {
			BuildData* build_data = static_cast<BuildData*>(cmd_list->commands_[0]);
			build_manager_->createBuilding(false, build_data->x, build_data->y, build_data->sender_id, build_data->build_kind);
			break;
		}
		case kDataPackageKind_Unit: {
			UnitData* unit_data = static_cast<UnitData*>(cmd_list->commands_[0]);
			// Check if not created, create it if not, update else
			if (unit_manager_->isUnitCreated(unit_data->sender_id, unit_data->id)) {
				unit_manager_->updateUnit(false, *unit_data);
			}
			else {
				unit_manager_->createUnit(false, unit_data->sender_id);
			}
			break;
		}
		case kDataPackageKind_CastleLife: {
			CastleLife* castle_life = static_cast<CastleLife*>(cmd_list->commands_[0]);
			updateCastleLife(false, castle_life->player_id, castle_life->new_life);
			break;
		}
		case kDataPackageKind_EndGame: {
			EndGame* end_game = static_cast<EndGame*>(cmd_list->commands_[0]);
			updateGameWinCondition(false, end_game->winner_id);
			break;
		}
		default: {
			break;
		}
		}
		
		// Delete first command
		cmd_list->commands_.erase(cmd_list->commands_.cbegin());
	}

	delete cmd_list;

}

// ------------------------------------------------------------------------- //
