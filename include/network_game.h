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

#include "game_script/buildings.h"

#include <../deps/SDL2-2.0.14/include/SDL.h>
#include <../deps/SDL2_image-2.0.5/include/SDL_image.h>

// ------------------------------------------------------------------------- //

class NetworkGame {
public:
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

	int client_id_;

private:
	NetworkGame();

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
	GameObject* mouse_build_object_;
	BuildManager* build_manager_;
	bool build_mode_;

public:
	// Network specific
	CommandList* cmd_list_; // Cmds to send to the server
	CommandList* recv_cmd_list_; // Cmds to send to the server

	friend class TextureLoader;
	friend class Tilemap;
	friend class GameObject;
	friend class Sprite;
	friend class Label;

};

// ------------------------------------------------------------------------- //

#endif // __NETWORK_GAME_H__