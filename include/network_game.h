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
#include "game_menus.h"

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
	void update(uint32_t time_step);
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
	GameMenus* game_menus_;
	BuildManager* build_manager_;

public:
	// Network specific
	CommandList* cmd_list_; // Cmds to send to the server
	CommandList* recv_cmd_list_; // Cmds to send to the server


	// Used to access private members of this class without constantly using a get method
	friend class TextureLoader;
	friend class GameObject;
	friend class GameMenus;
	friend class Tilemap;
	friend class Sprite;
	friend class Label;
	friend class Agent;
	friend class Path;
	friend class Box;

};

// ------------------------------------------------------------------------- //

#endif // __NETWORK_GAME_H__