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

	window_should_close_ = false;

	p1 = { 0, 0 };
	p2 = { 0, 0 };

}

// ------------------------------------------------------------------------- //

NetworkGame::~NetworkGame() {

}

// ------------------------------------------------------------------------- //

NetworkGame& NetworkGame::instance(){
	
	static NetworkGame* instance = new NetworkGame();
	return *instance;

}

// ------------------------------------------------------------------------- //

void NetworkGame::init() {

	SDL_Init(SDL_INIT_EVERYTHING);
	

	window_ = SDL_CreateWindow("Network game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 608, 400, SDL_WINDOW_SHOWN);
	NetworkGame::instance().window_ = window_;
	

	renderer_ = SDL_CreateRenderer(window_, -1, 0);
	SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
	NetworkGame::instance().renderer_ = renderer_;


	tex_loader_ = new TextureLoader();
	NetworkGame::instance().tex_loader_ = tex_loader_;


	scene_ = new Scene();
	scene_->init();
	NetworkGame::instance().scene_ = scene_;

}

// ------------------------------------------------------------------------- //

void NetworkGame::loadResources() {

	// Sprites, maps and everything
	/*tex_loader_->loadTexture("../../../data/images/tileset.png");
	tex_loader_->loadTexture("../../../data/images/terrain.png");
	tex_loader_->loadTexture("../../../data/images/objects.png");*/

}

// ------------------------------------------------------------------------- //

void NetworkGame::loadGame() {

	// Create game objects and map.
	//Scene* scene = scene_;

	build_mode_ = false;

	mouse_build_object_ = GameObject::CreateGameObject();
	Sprite* sprite;
	if (client_id_ == 2) {
		sprite = new Sprite(*mouse_build_object_, "../../../data/images/objects.png", 80, 48, 16, 32);
	}
	else {
		sprite = new Sprite(*mouse_build_object_, "../../../data/images/terrain.png", 192, 352, 32, 32);
	}
	mouse_build_object_->transform_.position_ = glm::vec3(-20.0f, -20.0f, 0);
	mouse_build_object_->addComponent(sprite);
	mouse_build_object_->active_ = false;

	/*GameObject* g2 = GameObject::CreateGameObject();
	g2->transform_.position_ = glm::vec3(480.0f + 8.0f, 224.0f, 0);
	g2->addComponent(sprite2);*/

}

// ------------------------------------------------------------------------- //

void NetworkGame::input() {

	SDL_PollEvent(&events_);
	switch (events_.type) {
	case SDL_QUIT: {
		window_should_close_ = true;
		break;
	}
	case SDL_KEYDOWN: {
		/* Check the SDLKey values and move change the coords */
		switch (events_.key.keysym.sym) {
		case SDLK_b: {
			build_mode_ = !build_mode_;
			scene_->map_->draw_grid_ = !scene_->map_->draw_grid_;
			if (build_mode_) {
				mouse_build_object_->active_ = true;
			}
			else {
				mouse_build_object_->active_ = false;
			}
			break;
		}
		default: {
			break;
		}
		}
		break;
	}
	case SDL_MOUSEBUTTONDOWN: {
		if (events_.button.button == SDL_BUTTON_LEFT) {
			if (build_mode_) {
				// Check if the current tile is buildable by this player
				int tile_value = basic_map[(transformed_mouse_x_ / 16) +
					((transformed_mouse_y_ / 16) * 38)];
				if (client_id_ == 2) {
					if (tile_value == p2_build_tiles[0]) {
						GameObject* build_object = GameObject::CreateGameObject();
						Sprite* sprite2 = new Sprite(*build_object, "../../../data/images/objects.png", 80, 48, 16, 32);
						build_object->transform_.position_ = glm::vec3(transformed_mouse_x_, transformed_mouse_y_, 0);
						build_object->addComponent(sprite2);
					}
				}
				else {
					if (tile_value == p1_build_tiles[0]) {
						// Create the object in the position of the mouse if it is allowed
						GameObject* build_object = GameObject::CreateGameObject();
						Sprite* sprite = new Sprite(*build_object, "../../../data/images/terrain.png", 192, 352, 32, 32);
						build_object->transform_.position_ = glm::vec3(transformed_mouse_x_, transformed_mouse_y_, 0);
						build_object->addComponent(sprite);
					}
				}
			}
		}
		break;
	}
	default: {
		break;
	}
	}

	SDL_GetMouseState(&mouse_pos_x_, &mouse_pos_y_);

}

// ------------------------------------------------------------------------- //

void NetworkGame::update() {

	if (build_mode_) {
		// Fix position with map tiling
		transformed_mouse_x_ = mouse_pos_x_ - (mouse_pos_x_ % 16) + 8.0f;
		transformed_mouse_y_ = mouse_pos_y_ - (mouse_pos_y_ % 16);

		mouse_build_object_->transform_.position_ = glm::vec3(transformed_mouse_x_,
			transformed_mouse_y_, 0.0f);
	}

	scene_->update();
	
}

// ------------------------------------------------------------------------- //

void NetworkGame::draw() {

	// Draw things
	scene_->draw();

	SDL_RenderPresent(renderer_);
	SDL_RenderClear(renderer_);

}

// ------------------------------------------------------------------------- //

void NetworkGame::close() {

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
