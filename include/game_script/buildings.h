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
#include "box.h"

// ------------------------------------------------------------------------- //

class Building;
class Caltrops;

enum BuildKind {
	kBuildKind_DefenseTower = 0,
	kBuildKind_House = 1,
	kBuildKind_Farm = 2,
	kBuildKind_WoodHouse = 3,
	kBuildKind_Offensive_Caltrops = 4,
	kBuildKind_Decorative_Hay = 5,
	kBuildKind_Decorative_ChoppedTree = 6,
};

class BuildManager {
public:
	BuildManager();
	~BuildManager();

	void init(int client_id);

	void selectBuilding(BuildKind build_kind);

	void createBuilding(bool send_command, int pos_x, int pos_y, int player_id, int build_kind, bool initial_building = false, int tile_kind_to_build = 3);
	bool checkTileToBuild(int client_id, int tile_value, glm::vec2 pos, int tile_kind_to_check);

	bool checkAndUseResourcesRequired(bool waste_resources);
	int addResourcesEarned(int pos_x, int pos_y, int build_kind);

	bool checkForBuilding(glm::vec2 build_pos, int player_id);
	int checkForSurroundingBuildings(glm::vec2 build_pos, int player_id);

	Sprite* getBuildingSprite(GameObject& go, BuildKind build_kind, int player_id);

	void updateBuildings(int client_id);// add the resources for all buildings
	void updateCaltrops(int client_id);

	void updateCaltrop(int client_id, int caltrop_id, int active);

	std::vector<Building*> p1_buildings;
	std::vector<Caltrops*> p1_caltrops;
	std::vector<Building*> p2_buildings;
	std::vector<Caltrops*> p2_caltrops;

	GameObject* mouse_build_object_;
	BuildKind selected_build_;

	int people_pieces_;
	int food_pieces_;
	int wood_pieces_;

protected:
	Building* createBuildingGameObject(int player_id, glm::vec2 pos, int build_kind);
	
	void buildAroundTile(glm::vec2 pos, BuildKind decorative_build_kind, int client_id, int tile_to_build);

	Sprite* build_sprites_[4];

};

// ------------------------------------------------------------------------- //

class Building : public GameObject {
public:
	~Building();

	int client_owner_id_;
	int build_id_;

	virtual void update() {};
	virtual void setResources(int new_tick_resources) {};

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

	static const int kPeopleCost = 2;
	static const int kFoodCost = 0;
	static const int kWoodCost = 4;

	virtual void update(uint32_t time_step) override; // each frame
	virtual void draw() override; // each frame


protected:


};

// ------------------------------------------------------------------------- //

class House : public Building {
public:
	House();
	~House();

	static const int kPeopleGain = 4;

	static const int kPeopleCost = 0;
	static const int kFoodCost = 6;
	static const int kWoodCost = 3;

	//virtual void update(uint32_t time_step) override; // each frame
	//void update(); // Only when server send a tick

protected:

};

// ------------------------------------------------------------------------- //

class Farm : public Building {
public:
	Farm();
	~Farm();

	static const int kPeopleCost = 4;
	static const int kFoodCost = 0;
	static const int kWoodCost = 8;

	//virtual void update(uint32_t time_step) override; // each frame
	virtual void update() override; // Only when server send a tick
	virtual void setResources(int new_tick_resources) override;

	int tick_resources_;

protected:

};

// ------------------------------------------------------------------------- //

class Woodhouse : public Building {
public:
	Woodhouse();
	~Woodhouse();

	static const int kPeopleCost = 3;
	static const int kFoodCost = 0;
	static const int kWoodCost = 10;

	//virtual void update(uint32_t time_step) override; // each frame
	virtual void update() override; // Only when server send a tick
	virtual void setResources(int new_tick_resources) override;

	int tick_resources_;

protected:

};

// ------------------------------------------------------------------------- //

class Caltrops : public Building {
public:
	Caltrops();
	~Caltrops();

	static const int kRoundsToSpawnCaltrops = 2;

	virtual void update() override; // Only when server send a tick
	virtual void draw() override; // each frame

	void manualFrameUpdate(); //Called manually only for the needed ones on each client

	void setCollisionBox();

	int rounds_to_respawn_;
	Box collision_box_;

protected:

};

// ------------------------------------------------------------------------- //

class Hay : public Building {
public:
	Hay();
	~Hay();


protected:

};

// ------------------------------------------------------------------------- //

class ChoppedTree : public Building {
public:
	ChoppedTree();
	~ChoppedTree();


protected:

};

// ------------------------------------------------------------------------- //

#endif // __BUILDINGS_H__