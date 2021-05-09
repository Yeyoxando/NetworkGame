/*
 *	Author: Diego Ochando Torres
 *  Date: 19/03/2021
 *  e-mail: c0022981@my.shu.ac.uk | yeyoxando@gmail.com
 */

// ------------------------------------------------------------------------- //

#include "network_game.h"

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
	Scene* scene = scene_;

	/*GameObject* g1 = GameObject::CreateGameObject();
	Sprite* sprite = new Sprite(*g1, "../../../data/images/terrain.png", 192, 352, 32, 32);
	g1->transform_.position_ = glm::vec3(96.0f + 8.0f
		, 160.0f, 0);
	g1->addComponent(sprite);*/

	GameObject* g2 = GameObject::CreateGameObject();
	Sprite* sprite2 = new Sprite(*g2, "../../../data/images/objects.png", 80, 48, 16, 32);
	g2->transform_.position_ = glm::vec3(480.0f + 8.0f, 224.0f, 0);
	g2->addComponent(sprite2);

	/*for (int y = 0; y < 25; ++y) {
		for (int x = 0; x < 38; ++x) {
			GameObject* g2 = GameObject::CreateGameObject();
			Sprite* sprite2;
			if (x == 20 && y == 10) {
				sprite2 = new Sprite(*g2, "../../../data/images/objects.png", 96, 0, 16, 16);
			}
			else {
				sprite2 = new Sprite(*g2, "../../../data/images/terrain.png", 48, 0, 16, 16);
			}
			g2->transform_.position_ = glm::vec3(8.0f + x * 16.0f, 8.0f + y * 16.0f, 0);
			g2->addComponent(sprite2);
		}
	}*/

}

// ------------------------------------------------------------------------- //

void NetworkGame::input() {

	SDL_PollEvent(&events_);
	switch (events_.type) {
	case SDL_QUIT: {
		window_should_close_ = true;
		break;
	}
	default: {
		break;
	}
	}

}

// ------------------------------------------------------------------------- //

void NetworkGame::update() {

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
