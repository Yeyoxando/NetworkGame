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
	
	window_ = SDL_CreateWindow("Network game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 400, SDL_WINDOW_SHOWN);
	
	renderer_ = SDL_CreateRenderer(window_, -1, 0);
	SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);

	scene_ = new Scene();
	scene_->init();

	NetworkGame::instance().window_ = window_;
	NetworkGame::instance().renderer_ = renderer_;
	NetworkGame::instance().scene_ = scene_;

}

// ------------------------------------------------------------------------- //

void NetworkGame::loadResources() {

	// Sprites, maps and everything

}

// ------------------------------------------------------------------------- //

void NetworkGame::loadGame() {

	// Create game objects and map.
	Scene* scene = scene_;

	GameObject* g1 = GameObject::CreateGameObject();
	Sprite* sprite = new Sprite(*g1, "tileset.png");
	sprite->set_pivotPoint(kPivotPoint_Center);
	g1->addComponent(sprite);

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

	SDL_DestroyRenderer(renderer_);
	SDL_DestroyWindow(window_);

	SDL_Quit();

}

// ------------------------------------------------------------------------- //

Scene* NetworkGame::getScene(){

	return scene_;

}

// ------------------------------------------------------------------------- //
