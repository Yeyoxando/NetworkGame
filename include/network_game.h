/*
 *	Author: Diego Ochando Torres
 *  Date: 19/03/2021
 *  e-mail: c0022981@my.shu.ac.uk | yeyoxando@gmail.com
 */

#ifndef __NETWORK_GAME_H__
#define __NETWORK_GAME_H__

// ------------------------------------------------------------------------- //

#include "game_data.h"

#include <../deps/SDL2-2.0.14/include/SDL.h>
#include <../deps/SDL2_image-2.0.5/include/SDL_image.h>

// ------------------------------------------------------------------------- //

class NetworkGame {
public:
	NetworkGame();
	~NetworkGame();

	void init();

	void loadResources();
	void startGame();

	void input();
	void update();
	void draw();

	void close();

	bool window_should_close_;

	Transform p1;
	Transform p2;

private:
	SDL_Window* window_;
	SDL_Renderer* renderer_;

	SDL_Event events_;

};

// ------------------------------------------------------------------------- //

#endif // __NETWORK_GAME_H__