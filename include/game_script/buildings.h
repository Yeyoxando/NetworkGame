/*
 *	Author: Diego Ochando Torres
 *  Date: 09/05/2021
 *  e-mail: c0022981@my.shu.ac.uk | yeyoxando@gmail.com
 */

#ifndef __BUILDINGS_H__
#define __BUILDINGS_H__

// ------------------------------------------------------------------------- //

#include <vector>

#include "game_object.h"

// ------------------------------------------------------------------------- //

class Building;

enum BuildKind {
	kBuildKind_DefenseTower = 0,
	kBuildKind_House = 1,
	kBuildKind_Farm = 2,
	kBuildKind_WoodHouse = 3,
};

class BuildManager {
public:
	BuildManager();
	~BuildManager();

	void selectBuilding(BuildKind build_kind);

	void createBuilding(bool send_command, int pos_x, int pos_y, int player_id, int build_kind);

	bool checkAndUseResourcesRequired(bool waste_resources);
	void addResourcesEarned(int pos_x, int pos_y);

	bool checkForBuilding(glm::vec2 build_pos);
	bool checkForSurroundingBuildings(glm::vec2 build_pos);

	Sprite* getBuildingSprite(GameObject& go, BuildKind build_kind, int player_id);

	std::vector<Building*> p1_buildings;
	std::vector<Building*> p2_buildings;

	GameObject* mouse_build_object_;
	BuildKind selected_build_;

	int people_pieces_;
	int food_pieces_;
	int wood_pieces_;

protected:
	Sprite* build_sprites_[4];

};

// ------------------------------------------------------------------------- //

class Building {
public: 
	Building();
	~Building();

protected:

};

// ------------------------------------------------------------------------- //

#endif // __BUILDINGS_H__