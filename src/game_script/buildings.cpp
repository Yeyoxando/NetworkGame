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

void BuildManager::selectBuilding(BuildKind build_kind){

	mouse_build_object_->removeComponent(ComponentKind::kComponentKind_Sprite);

	mouse_build_object_->addComponent(build_sprites_[(int)build_kind]);

	selected_build_ = build_kind;

}

// ------------------------------------------------------------------------- //

void BuildManager::createBuilding(bool send_command, int pos_x, int pos_y, int player_id, int build_kind){

	// Check if the current tile is buildable by this player
	int tile_value = basic_map[(pos_x / 16) +	((pos_y / 16) * 38)];

	if (player_id == 2) {
		if (tile_value == p2_build_tiles[0]) {

			GameObject* build_object = GameObject::CreateGameObject();
			Sprite* sprite = getBuildingSprite(*build_object, (BuildKind)build_kind, player_id);
			build_object->addComponent(sprite);

			if (build_kind == kBuildKind_DefenseTower) {
				build_object->transform_.position_ = glm::vec3(pos_x, pos_y, 0.0f);
			}
			else {
				build_object->transform_.position_ = glm::vec3(pos_x, pos_y + 8.0f, 0.0f);
			}

			if (send_command) {
				// Add command to the cmd list for when the net thread starts working again
				BuildData* build_cmd = CreateBuildData(player_id, glm::vec2(pos_x, pos_y), build_kind);
				NetworkGame::instance().cmd_list_->commands_.push_back(build_cmd);
			}
		}
	}
	else {
		if (tile_value == p1_build_tiles[0]) {
			// Create the object in the position of the mouse if it is allowed
			GameObject* build_object = GameObject::CreateGameObject();
			Sprite* sprite = getBuildingSprite(*build_object, (BuildKind)build_kind, player_id);
			build_object->addComponent(sprite);

			if (build_kind == kBuildKind_DefenseTower) {
				build_object->transform_.position_ = glm::vec3(pos_x, pos_y, 0.0f);
			}
			else {
				build_object->transform_.position_ = glm::vec3(pos_x, pos_y + 8.0f, 0.0f);
			}

			if (send_command) {
				// Add command to the cmd list for when the net thread starts working again
				BuildData* build_cmd = CreateBuildData(player_id, glm::vec2(pos_x, pos_y), build_kind);
				NetworkGame::instance().cmd_list_->commands_.push_back(build_cmd);
			}
		}
	}

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