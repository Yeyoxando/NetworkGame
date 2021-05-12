/*
 *	Author: Diego Ochando Torres
 *  Date: 10/05/2021
 *  e-mail: c0022981@my.shu.ac.uk | yeyoxando@gmail.com
 */

#ifndef __GAME_MENUS_H__
#define __GAME_MENUS_H__

// ------------------------------------------------------------------------- //

#include "imgui.h"
#include <sdl_events.h>

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

private:
	void createBasicMenu();
	void createBuildingMenu();
	void createUnitsMenu();

	// void createMainMenu()
	// void createWinMenu()
	// void createLoseMenu()
	// void createDisconnectMenu()
	// void createWaitingForConnectionMenu()
	// void create...Menu()


	ImGuiWindowFlags window_flags_;

};

// ------------------------------------------------------------------------- //

#endif // __GAME_MENUS_H__ 