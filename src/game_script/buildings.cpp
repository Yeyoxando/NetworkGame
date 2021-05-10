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

}

// ------------------------------------------------------------------------- //

BuildManager::~BuildManager(){

	while (p1_buildings.size() > 0) {
		p1_buildings.erase(p1_buildings.cbegin());
	}

	while (p2_buildings.size() > 0) {
		p2_buildings.erase(p2_buildings.cbegin());
	}

}

// ------------------------------------------------------------------------- //

void BuildManager::createBuilding(bool send_command, int pos_x, int pos_y, int player_id, int build_kind){

	// Check if the current tile is buildable by this player
	int tile_value = basic_map[(pos_x / 16) +	((pos_y / 16) * 38)];

	if (player_id == 2) {
		if (tile_value == p2_build_tiles[0]) {

			GameObject* build_object = GameObject::CreateGameObject();
			Sprite* sprite2 = new Sprite(*build_object, "../../../data/images/objects.png", 80, 48, 16, 32);
			build_object->transform_.position_ = glm::vec3(pos_x, pos_y, 0.0f);
			build_object->addComponent(sprite2);
			
			/*Building* new_build = new Building();
			new_build->go_ref_ = build_object;
			new_build->build_kind_ = (Building::BuildKind)build_kind;
			new_build->pos_x_ = pos_x;
			new_build->pos_y_ = pos_y;
			p2_buildings.push_back(new_build);*/


			if (send_command) {
				// Add command to the cmd list for when the net thread starts working again
				BuildData* build_cmd = CreateBuildData(player_id, glm::vec2(pos_x, pos_y));
				NetworkGame::instance().cmd_list_->commands_.push_back(build_cmd);
			}
		}
	}
	else {
		if (tile_value == p1_build_tiles[0]) {
			// Create the object in the position of the mouse if it is allowed
			GameObject* build_object = GameObject::CreateGameObject();
			Sprite* sprite = new Sprite(*build_object, "../../../data/images/terrain.png", 192, 352, 32, 32);
			build_object->transform_.position_ = glm::vec3(pos_x, pos_y, 0.0f);
			build_object->addComponent(sprite);

			/*Building* new_build = new Building();
			new_build->go_ref_ = build_object;
			new_build->build_kind_ = (Building::BuildKind)build_kind;
			new_build->pos_x_ = pos_x;
			new_build->pos_y_ = pos_y;
			p2_buildings.push_back(new_build);*/


			if (send_command) {
				// Add command to the cmd list for when the net thread starts working again
				BuildData* build_cmd = CreateBuildData(player_id, glm::vec2(pos_x, pos_y));
				NetworkGame::instance().cmd_list_->commands_.push_back(build_cmd);
			}
		}
	}

}

// ------------------------------------------------------------------------- //

// ------------------------------------------------------------------------- //

// ------------------------------------------------------------------------- //

Building::Building(){



}

// ------------------------------------------------------------------------- //

Building::~Building(){



}

// ------------------------------------------------------------------------- //
