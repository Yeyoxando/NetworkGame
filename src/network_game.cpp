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

	scene_->init();

}

// ------------------------------------------------------------------------- //

void NetworkGame::loadResources() {

	// Sprites, maps and everything

}

// ------------------------------------------------------------------------- //

void NetworkGame::loadGame() {

	// Create game objects and map.

	GameObject* g1 = GameObject::CreateGameObject();


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

	SDL_RenderClear(renderer_);
	SDL_RenderPresent(renderer_);

}

// ------------------------------------------------------------------------- //

void NetworkGame::close() {

	scene_->finish();

	SDL_DestroyRenderer(renderer_);
	SDL_DestroyWindow(window_);

	SDL_Quit();

}

// ------------------------------------------------------------------------- //

Scene* NetworkGame::getScene(){

	return scene_;

}

// ------------------------------------------------------------------------- //
