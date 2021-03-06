/*
 *	Author: Diego Ochando Torres
 *  Date: 10/05/2021
 *  e-mail: c0022981@my.shu.ac.uk | yeyoxando@gmail.com
 */

#ifndef __GAME_MENUS_H__
#define __GAME_MENUS_H__

// ------------------------------------------------------------------------- //

#include "imgui.h"
#include <SDL.h>

// ------------------------------------------------------------------------- //

class GameMenus {
public:
	GameMenus();
	~GameMenus();

	void initGUI();

	void inputGUI(SDL_Event& event);
	void manageGUI();
	void drawGUI();

	void shutdownGUI();


	bool debug_mode_;
	bool build_mode_;
	bool units_mode_;

private:
	void drawBasicMenu();
	void drawBuildingMenu();
	void drawUnitsMenu();

	void drawWinMenu();
	void drawLoseMenu();

	void drawDisconnectionMenu();

	// void drawWaitingForConnectionMenu()
	void drawConnectionMenu();

	void resourcesInfo();
	void playersInfo();

	ImGuiWindowFlags window_flags_;

	SDL_Texture* tower_texture_;
	SDL_Texture* house_texture_;
	SDL_Texture* farm_texture_;
	SDL_Texture* woodhouse_texture_;

	SDL_Texture* unit_texture_;

	SDL_Texture* winner_texture_;
	SDL_Texture* loser_texture_;

	SDL_Texture* resource_people_texture_;
	SDL_Texture* resource_food_texture_;
	SDL_Texture* resource_wood_texture_;
	SDL_Texture* resource_units_texture_;

};

// ------------------------------------------------------------------------- //

#endif // __GAME_MENUS_H__ 