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
#include "game_script/units.h"

#include <SDL.h>
#include <SDL_image.h>

// ------------------------------------------------------------------------- //

class NetworkGame {
public:
	~NetworkGame();

	static NetworkGame& instance();

	void init();
	void loadGame();

	void input();
	void update(uint32_t time_step);
	void draw();

	void close();

	Scene* getScene();

	bool window_should_close_;
	int client_id_;

	bool game_end_;
	bool winner_;
	bool loser_;

	bool disconnected_player_;

	bool custom_ip_;
	char* ip_;

	void updateCastleLife(bool send_command, int player_id, int castle_life);
	void updateGameWinCondition(bool send_command, int winner_id);

private:
	NetworkGame();

	void processNetCommands();

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

	bool game_started_;

	//Game specific
	BuildManager* build_manager_;
	UnitManager* unit_manager_;
	GameMenus* game_menus_;

	int castle_life_p1_;
	int castle_life_p2_;

public:
	// Network specific
	CommandList* cmd_list_; // Cmds to send to the server
	CommandList* recv_cmd_list_; // Cmds to send to the server


	// Used to access private members of this class without constantly using a get method
	friend class TextureLoader;
	friend class UnitManager;
	friend class GameObject;
	friend class GameMenus;
	friend class Tilemap;
	friend class Sprite;
	friend class Label;
	friend class Agent;
	friend class Path;
	friend class Box;
	friend class Farm;
	friend class Woodhouse;
	friend class Caltrops;

};

// ------------------------------------------------------------------------- //

#endif // __NETWORK_GAME_H__