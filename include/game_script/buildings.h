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
	int addResourcesEarned(int pos_x, int pos_y);

	bool checkForBuilding(glm::vec2 build_pos);
	bool checkForSurroundingBuildings(glm::vec2 build_pos);

	Sprite* getBuildingSprite(GameObject& go, BuildKind build_kind, int player_id);

	void updateBuildings(int client_id);// add the resources for all buildings

	std::vector<Building*> p1_buildings;
	std::vector<Building*> p2_buildings;

	GameObject* mouse_build_object_;
	BuildKind selected_build_;

	int people_pieces_;
	int food_pieces_;
	int wood_pieces_;

protected:
	void createBuildingGameObject(int player_id, glm::vec2 pos, int build_kind);

	Sprite* build_sprites_[4];

};

// ------------------------------------------------------------------------- //

class Building : public GameObject {
public:
	~Building();

	int client_owner_id_;
	int build_id_;

	virtual void update() {};

	virtual void update(uint32_t time_step) override { GameObject::update(time_step); }
	virtual void draw() override { GameObject::draw(); }

protected:
	Building();
	BuildKind build_kind_;

};

// ------------------------------------------------------------------------- //

class Tower : public Building {
public:
	Tower();
	~Tower();

	const int kPeopleCost = 2;
	const int kFoodCost = 0;
	const int kWoodCost = 3;

	virtual void update(uint32_t time_step) override; // each frame
	virtual void draw() override; // each frame

	int range_;

protected:


};

// ------------------------------------------------------------------------- //

class House : public Building {
public:
	House();
	~House();

	const int kPeopleCost = 0;
	const int kFoodCost = 3;
	const int kWoodCost = 2;

	//virtual void update(uint32_t time_step) override; // each frame
	//void update(); // Only when server send a tick

protected:

};

// ------------------------------------------------------------------------- //

class Farm : public Building {
public:
	Farm();
	~Farm();

	const int kPeopleCost = 2;
	const int kFoodCost = 0;
	const int kWoodCost = 4;

	//virtual void update(uint32_t time_step) override; // each frame
	virtual void update() override; // Only when server send a tick

	int tick_resources_;

protected:

};

// ------------------------------------------------------------------------- //

class Woodhouse : public Building {
public:
	Woodhouse();
	~Woodhouse();

	const int kPeopleCost = 2;
	const int kFoodCost = 0;
	const int kWoodCost = 3;

	//virtual void update(uint32_t time_step) override; // each frame
	virtual void update() override; // Only when server send a tick

	int tick_resources_;

protected:

};

// ------------------------------------------------------------------------- //

#endif // __BUILDINGS_H__