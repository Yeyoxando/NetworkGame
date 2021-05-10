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

class BuildManager {
public:
	BuildManager();
	~BuildManager();

	void createBuilding(bool send_command, int pos_x, int pos_y, int player_id, int build_kind);

	bool checkForBuildings(glm::vec2 building_pos);

	std::vector<glm::vec2> built_tiles_;
protected:

};

// ------------------------------------------------------------------------- //

class Building {
public:
	Building();
	virtual ~Building();

	enum BuildKind {
		kBuildKind_DefenseTower = 0,
		kBuildKind_House = 1,
		kBuildKind_WoodHouse = 2,
		kBuildKind_Farm = 3
	};

	int pos_x_;
	int pos_y_;
	BuildKind build_kind_;
	GameObject* go_ref_;

};

// ------------------------------------------------------------------------- //

class DefenseTower : public Building {
public:
	DefenseTower();
	virtual ~DefenseTower();

	int damage_;
	int range_;
protected:

};

// ------------------------------------------------------------------------- //

class House : public Building {
public:
	House();
	virtual ~House();

protected:

};

// ------------------------------------------------------------------------- //

class WoodHouse : public Building {
public:
	WoodHouse();
	virtual ~WoodHouse();

	int surrounding_wood_tiles_;
protected:

};

// ------------------------------------------------------------------------- //

class Farm : public Building {
public:
	Farm();
	virtual ~Farm();

	int surrounding_grass_tiles_;
	int surrounding_ground_tiles_;
protected:

};

// ------------------------------------------------------------------------- //

#endif // __BUILDINGS_H__