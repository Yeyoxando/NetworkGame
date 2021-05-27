/*
 *	Author: Diego Ochando Torres
 *  Date: 09/05/2021
 *  e-mail: c0022981@my.shu.ac.uk | yeyoxando@gmail.com
 */

// ------------------------------------------------------------------------- //

#include "game_script/buildings.h"
#include "network_game_maps.h"
#include "network_game.h"
#include "network_data.h"
#include "game_object.h"

#include <glm.hpp>

// ------------------------------------------------------------------------- //

BuildManager::BuildManager(){

	p1_buildings = std::vector<Building*>(0);
	p2_buildings = std::vector<Building*>(0);


	mouse_build_object_ = GameObject::CreateGameObject();

	if (NetworkGame::instance().client_id_ == 2) {
		build_sprites_[0] = new Sprite(*mouse_build_object_, "../../../data/images/objects.png", 80, 48, 16, 32);
		build_sprites_[1] = new Sprite(*mouse_build_object_, "../../../data/images/objects.png", 64, 0, 16, 16);
		build_sprites_[2] = new Sprite(*mouse_build_object_, "../../../data/images/objects.png", 96, 0, 16, 16);
		build_sprites_[3] = new Sprite(*mouse_build_object_, "../../../data/images/objects.png", 80, 16, 16, 16);
	}
	else {
		build_sprites_[0] = new Sprite(*mouse_build_object_, "../../../data/images/terrain.png", 192, 352, 32, 32);
		build_sprites_[1] = new Sprite(*mouse_build_object_, "../../../data/images/objects.png", 48, 0, 16, 16);
		build_sprites_[2] = new Sprite(*mouse_build_object_, "../../../data/images/objects.png", 112, 0, 16, 16);
		build_sprites_[3] = new Sprite(*mouse_build_object_, "../../../data/images/objects.png", 80, 0, 16, 16);
	}
	mouse_build_object_->transform_.position_ = glm::vec3(-20.0f, -20.0f, 0);
	mouse_build_object_->addComponent(build_sprites_[0]);
	mouse_build_object_->active_ = false;

	selected_build_ = kBuildKind_DefenseTower;

	people_pieces_ = 0; // 3
	food_pieces_ = 0; // 8
	wood_pieces_ = 0; // 5

}

// ------------------------------------------------------------------------- //

BuildManager::~BuildManager(){

	while (p1_buildings.size() > 0) {
		p1_buildings.erase(p1_buildings.cbegin());
	}

	while (p2_buildings.size() > 0) {
		p2_buildings.erase(p2_buildings.cbegin());
	}
	/*
	for (int i = 0; i < 4; ++i) {
		delete build_sprites_[i];
	}
	*/

}

// ------------------------------------------------------------------------- //

void BuildManager::init(int client_id) {

	//Create initial players buildings, as the resource cost has to be balanced and you
	// cannot build everything from start

	// 1 house -> 4 people? instead of 3
	if (client_id == 2) {
		createBuilding(true, 520, 80, client_id, (int)kBuildKind_House, true);
		createBuilding(true, 328, 112, client_id, (int)kBuildKind_Farm, true);
		createBuilding(true, 376, 272, client_id, (int)kBuildKind_WoodHouse, true);
	}
	else {
		createBuilding(true, 88, 320, client_id, (int)kBuildKind_House, true);
		createBuilding(true, 264, 288, client_id, (int)kBuildKind_Farm, true);
		createBuilding(true, 24, 64, client_id, (int)kBuildKind_WoodHouse, true);
	}
	// 1 farm -> 8 food
	// 1 woodhouse -> 5 wood

}

// ------------------------------------------------------------------------- //

void BuildManager::selectBuilding(BuildKind build_kind){

	mouse_build_object_->removeComponent(ComponentKind::kComponentKind_Sprite);

	mouse_build_object_->addComponent(build_sprites_[(int)build_kind]);

	selected_build_ = build_kind;

}

// ------------------------------------------------------------------------- //

void BuildManager::createBuilding(bool send_command, int pos_x, int pos_y, int player_id, int build_kind, bool initial_building){

	// Get current tile value for the mouse position
	int tile_value = basic_map[(pos_x / 16) +	((pos_y / 16) * 38)];

	glm::vec2 pos = glm::vec2(pos_x, pos_y);

	if (player_id == 2) { // Player 2 Building
		// Previous comprobations for placement
		if (tile_value == p2_build_tiles[0] && !checkForBuilding(pos, player_id)) {
			if (build_kind == kBuildKind_DefenseTower) {
				if (NetworkGame::instance().getScene()->map_->
						checkFourAdjacentTiles(pos, Tilemap::kTileKind_RoadBuildable) == 0) return;
			}

			// If everything okay check for resources and waste them if possible
			bool waste_resources = send_command;
			if (initial_building) waste_resources = false;

			if (checkAndUseResourcesRequired(waste_resources)) {

				Building* new_build = createBuildingGameObject(player_id, pos, build_kind);

				if (send_command) {
					int tick_resources = addResourcesEarned(pos_x, pos_y, build_kind);
					new_build->setResources(tick_resources);
					// Add command to the cmd list for when the net thread starts working again
					BuildData* build_cmd = CreateBuildData(player_id, pos, build_kind);
					NetworkGame::instance().cmd_list_->commands_.push_back(build_cmd);
				}
			}
		}
	}
	else { // Player 1 Building
		// Previous comprobations for placement
		if (tile_value == p1_build_tiles[0] && !checkForBuilding(pos, player_id)) {
			if (build_kind == kBuildKind_DefenseTower) {
				if (NetworkGame::instance().getScene()->map_->
					checkFourAdjacentTiles(pos, Tilemap::kTileKind_RoadBuildable) == 0) return;
			}

			// If everything okay check for resources and waste them if possible
			bool waste_resources = send_command;
			if (initial_building) waste_resources = false;

			if (checkAndUseResourcesRequired(waste_resources)) {
				
				Building* new_build = createBuildingGameObject(player_id, pos, build_kind);

				if (send_command) {
					// Add command to the cmd list for when the net thread starts working again
					int tick_resources = addResourcesEarned(pos_x, pos_y, build_kind);
					new_build->setResources(tick_resources);
					BuildData* build_cmd = CreateBuildData(player_id, pos, build_kind);
					NetworkGame::instance().cmd_list_->commands_.push_back(build_cmd);
				}
			}
		}
	}

}

// ------------------------------------------------------------------------- //

bool BuildManager::checkAndUseResourcesRequired(bool waste_resources){

	if (!waste_resources) return true;

	switch (selected_build_){
	case kBuildKind_DefenseTower: {
		if (people_pieces_ >= Tower::kPeopleCost && wood_pieces_ >= Tower::kWoodCost) {
			people_pieces_ -= Tower::kPeopleCost;
			wood_pieces_ -= Tower::kWoodCost;
			return true;
		}
		break;
	}
	case kBuildKind_House: {
		if (food_pieces_ >= House::kFoodCost && wood_pieces_ >= House::kWoodCost) {
			food_pieces_ -= House::kFoodCost;
			wood_pieces_ -= House::kWoodCost;
			return true;
		}
		break;
	}
	case kBuildKind_Farm: {
		if (people_pieces_ >= Farm::kPeopleCost && wood_pieces_ >= Farm::kWoodCost) {
			people_pieces_ -= Farm::kPeopleCost;
			wood_pieces_ -= Farm::kWoodCost;
			return true;
		}
		break;
	}
	case kBuildKind_WoodHouse: {
		if (people_pieces_ >= Woodhouse::kPeopleCost && wood_pieces_ >= Woodhouse::kWoodCost) {
			people_pieces_ -= Woodhouse::kPeopleCost;
			wood_pieces_ -= Woodhouse::kWoodCost;
			return true;
		}
		break;
	}
	default: {
		return true;
		break;
	}
	}

	return false;

}

// ------------------------------------------------------------------------- //

int BuildManager::addResourcesEarned(int pos_x, int pos_y, int build_kind){

	Tilemap* map = NetworkGame::instance().getScene()->map_;
	int resources_earned = 0;

	switch (build_kind) {
	case kBuildKind_DefenseTower: {
		
		break;
	}
	case kBuildKind_House: {
		resources_earned = House::kPeopleGain;
		people_pieces_ += resources_earned;
		break;
	}
	case kBuildKind_Farm: {
		// surrounding grass tiles
		resources_earned = map->checkEightAdjacentTiles(glm::vec2(pos_x, pos_y), 
			Tilemap::kTileKind_FarmGrass);
		food_pieces_ += resources_earned;
		break;
	}
	case kBuildKind_WoodHouse: {
		// surrounding wood tiles
		resources_earned = map->checkEightAdjacentTiles(glm::vec2(pos_x, pos_y), 
			Tilemap::kTileKind_Tree);
		wood_pieces_ += resources_earned;
		break;
	}
	default: {
		break;
	}
	}

	return resources_earned;

}

// ------------------------------------------------------------------------- //

bool BuildManager::checkForBuilding(glm::vec2 build_pos, int player_id){

	int current_tile_value_x = (int)build_pos.x / 16;
	int current_tile_value_y = (int)build_pos.y / 16;
	int aux_tile_value_x, aux_tile_value_y;

	if (player_id == 2) {
		for (int i = 0; i < p2_buildings.size(); ++i) {
			aux_tile_value_x = (int)p2_buildings[i]->transform_.position_.x / 16;
			aux_tile_value_y = (int)p2_buildings[i]->transform_.position_.y / 16;
			if (aux_tile_value_x == current_tile_value_x &&
				aux_tile_value_y == current_tile_value_y) {
				return true;
			}
		}
	}
	else {
		for (int i = 0; i < p1_buildings.size(); ++i) {
			aux_tile_value_x = (int)p1_buildings[i]->transform_.position_.x / 16;
			aux_tile_value_y = (int)p1_buildings[i]->transform_.position_.y / 16;
			if (aux_tile_value_x == current_tile_value_x &&
				aux_tile_value_y == current_tile_value_y) {
				return true;
			}
		}
	}

	return false;

}

// ------------------------------------------------------------------------- //

int BuildManager::checkForSurroundingBuildings(glm::vec2 build_pos, int player_id){



	return 0;

}

// ------------------------------------------------------------------------- //

Sprite* BuildManager::getBuildingSprite(GameObject& go, BuildKind build_kind, int player_id){

	Sprite* sprite = nullptr;

	switch (build_kind) {
	case kBuildKind_DefenseTower: {
		if (player_id == 2) {
			sprite = new Sprite(go, "../../../data/images/objects.png", 80, 48, 16, 32);
		}
		else {
			sprite = new Sprite(go, "../../../data/images/terrain.png", 192, 352, 32, 32);
		}
		break;
	}
	case kBuildKind_House: {
		if (player_id == 2) {
			sprite = new Sprite(go, "../../../data/images/objects.png", 64, 0, 16, 16);
		}
		else {
			sprite = new Sprite(go, "../../../data/images/objects.png", 48, 0, 16, 16);
		}
		break;
	}
	case kBuildKind_Farm: {
		if (player_id == 2) {
			sprite = new Sprite(go, "../../../data/images/objects.png", 96, 0, 16, 16);
		}
		else {
			sprite = new Sprite(go, "../../../data/images/objects.png", 112, 0, 16, 16);
		}
		break;
	}
	case kBuildKind_WoodHouse: {
		if (player_id == 2) {
			sprite = new Sprite(go, "../../../data/images/objects.png", 80, 16, 16, 16);
		}
		else {
			sprite = new Sprite(go, "../../../data/images/objects.png", 80, 0, 16, 16);
		}
		break;
	}
	default: {
		break;
	}
	}

	return sprite;

}

// ------------------------------------------------------------------------- //

void BuildManager::updateBuildings(int client_id){

	if (client_id == 2) {
		for (int i = 0; i < p2_buildings.size(); ++i) {
			p2_buildings[i]->update(); // virtual override on building classes
		}
	}
	else {
		for (int i = 0; i < p1_buildings.size(); ++i) {
			p1_buildings[i]->update(); // virtual override on building classes
		}
	}

}

// ------------------------------------------------------------------------- //

Building* BuildManager::createBuildingGameObject(int player_id, glm::vec2 pos, int build_kind){

	Building* building = nullptr;

	switch (build_kind) {
	case kBuildKind_DefenseTower: {
		building = new Tower();
		building->init(glm::vec3(pos, 0.0f));
		Sprite* sprite = getBuildingSprite(*building, (BuildKind)build_kind, player_id);
		building->addComponent(sprite);
		buildAroundTile(pos, BuildKind::kBuildKind_Offensive_Caltrops, true);
		break;
	}
	case kBuildKind_House: {
		building = new House();
		building->init(glm::vec3(pos.x, pos.y + 8.0f, 0.0f));
		Sprite* sprite = getBuildingSprite(*building, (BuildKind)build_kind, player_id);
		building->addComponent(sprite);

		break;
	}
	case kBuildKind_Farm: {
		building = new Farm();
		building->init(glm::vec3(pos.x, pos.y + 8.0f, 0.0f));
		Sprite* sprite = getBuildingSprite(*building, (BuildKind)build_kind, player_id);
		building->addComponent(sprite);
		buildAroundTile(pos, BuildKind::kBuildKind_Decorative_Hay, false);
		break;
	}
	case kBuildKind_WoodHouse: {
		building = new Woodhouse();
		building->init(glm::vec3(pos.x, pos.y + 8.0f, 0.0f));
		Sprite* sprite = getBuildingSprite(*building, (BuildKind)build_kind, player_id);
		building->addComponent(sprite);
		buildAroundTile(pos, BuildKind::kBuildKind_Decorative_ChoppedTree, false);
		break;
	}
	case kBuildKind_Offensive_Caltrops: {
		building = new Caltrops();
		building->init(glm::vec3(pos.x, pos.y + 8.0f, 0.0f));

		break;
	}
	case kBuildKind_Decorative_Hay: {
		building = new Hay();
		building->init(glm::vec3(pos.x, pos.y + 8.0f, 0.0f));

		break;
	}
	case kBuildKind_Decorative_ChoppedTree: {
		building = new ChoppedTree();
		building->init(glm::vec3(pos.x, pos.y + 8.0f, 0.0f));

		break;
	}
	default: {
		break;
	}
	}

	building->client_owner_id_ = player_id;

	if (player_id == 2) {
		building->build_id_ = p2_buildings.size();
		p2_buildings.push_back(building);
	}
	else {
		building->build_id_ = p1_buildings.size();
		p1_buildings.push_back(building);
	}
	
	NetworkGame::instance().getScene()->addGameObject(building);

	return building;

}

// ------------------------------------------------------------------------- //

void BuildManager::buildAroundTile(glm::vec2 pos, BuildKind decorative_build_kind, 
	bool build_only_on_roads){

	// Dont send command, the other player will create the same buildings when the parent building is created.


}

// ------------------------------------------------------------------------- //

Building::Building(){

	build_id_ = -1;
	active_ = true;

}

// ------------------------------------------------------------------------- //

Building::~Building(){



}

// ------------------------------------------------------------------------- //

Tower::Tower(){

	build_kind_ = kBuildKind_DefenseTower;
	rounds_to_drop_ = kRoundsToSpawnCaltrops;

}

// ------------------------------------------------------------------------- //

Tower::~Tower(){



}

// ------------------------------------------------------------------------- //

void Tower::update(uint32_t time_step){

	GameObject::update(time_step);

	// Attack enemy units if they are near
	// enemy towers will attack in the enemy client, in this way the player units life 
	// will be sent with the position and everything else

}

// ------------------------------------------------------------------------- //

void Tower::draw(){

	GameObject::draw();

}

// ------------------------------------------------------------------------- //

House::House(){

	build_kind_ = kBuildKind_House;

}

// ------------------------------------------------------------------------- //

House::~House(){



}

// ------------------------------------------------------------------------- //

Farm::Farm() {

	build_kind_ = kBuildKind_Farm;
	tick_resources_ = 0; // Set when built

}

// ------------------------------------------------------------------------- //

Farm::~Farm(){



}

// ------------------------------------------------------------------------- //

void Farm::update() {

	// Give player this farms resources when the server ticks
	NetworkGame::instance().build_manager_->food_pieces_ += tick_resources_;

}

// ------------------------------------------------------------------------- //

void Farm::setResources(int new_tick_resources){

	tick_resources_ = new_tick_resources;

}

// ------------------------------------------------------------------------- //

Woodhouse::Woodhouse() {

	build_kind_ = kBuildKind_WoodHouse;
	tick_resources_ = 0; // Set when built

}

// ------------------------------------------------------------------------- //

Woodhouse::~Woodhouse(){



}

// ------------------------------------------------------------------------- //

void Woodhouse::update() {

	// Give player this woodhouse resources when the server ticks
	NetworkGame::instance().build_manager_->wood_pieces_ += tick_resources_;

}

// ------------------------------------------------------------------------- //

void Woodhouse::setResources(int new_tick_resources){

	tick_resources_ = new_tick_resources;

}

// ------------------------------------------------------------------------- //

Caltrops::Caltrops(){

	build_kind_ = kBuildKind_Offensive_Caltrops;
	level_ = 0;
	Sprite* sprite = new Sprite(*this, "../../../data/images/terrain.png", 256, 384, 16, 16);
	addComponent(sprite);

}

// ------------------------------------------------------------------------- //

Caltrops::~Caltrops(){



}

// ------------------------------------------------------------------------- //

Hay::Hay(){

	build_kind_ = kBuildKind_Decorative_Hay;
	Sprite* sprite = new Sprite(*this, "../../../data/images/terrain.png", 176, 464, 16, 16);
	addComponent(sprite);

}

// ------------------------------------------------------------------------- //

Hay::~Hay(){



}

// ------------------------------------------------------------------------- //

ChoppedTree::ChoppedTree() {

	build_kind_ = kBuildKind_Decorative_ChoppedTree;
	Sprite* sprite = new Sprite(*this, "../../../data/images/terrain.png", 272, 464, 16, 16);
	addComponent(sprite);

}

// ------------------------------------------------------------------------- //

ChoppedTree::~ChoppedTree(){



}

// ------------------------------------------------------------------------- //
