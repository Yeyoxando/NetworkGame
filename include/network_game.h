/*
 *	Author: Diego Ochando Torres
 *  Date: 19/03/2021
 *  e-mail: c0022981@my.shu.ac.uk | yeyoxando@gmail.com
 */

#ifndef __NETWORK_GAME_H__
#define __NETWORK_GAME_H__

// ------------------------------------------------------------------------- //

#include "network_data.h"
#include "scene.h"
#include "texture_loader.h"

#include <../deps/SDL2-2.0.14/include/SDL.h>
#include <../deps/SDL2_image-2.0.5/include/SDL_image.h>

// ------------------------------------------------------------------------- //

class NetworkGame {
public:
	NetworkGame();
	~NetworkGame();

	static NetworkGame& instance();

	void init();

	void loadResources();
	void loadGame();

	void input();
	void update();
	void draw();

	void close();

	Scene* getScene();

	bool window_should_close_;

	TransformTest p1;
	TransformTest p2;

	int client_id_;

private:
	// Engine specific
	SDL_Window* window_;
	SDL_Renderer* renderer_;

	SDL_Event events_;

	Scene* scene_;
	TextureLoader* tex_loader_;

	int mouse_pos_x_;
	int mouse_pos_y_;
	int transformed_mouse_x_;
	int transformed_mouse_y_;

	//Game specific
	bool build_mode_;
	GameObject* mouse_build_object_;

	friend class TextureLoader;
	friend class Tilemap;
	friend class GameObject;
	friend class Sprite;
	friend class Label;

};

// ------------------------------------------------------------------------- //

#endif // __NETWORK_GAME_H__