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

	built_tiles_ = std::vector<glm::vec2>(0);

}

// ------------------------------------------------------------------------- //

BuildManager::~BuildManager(){

	while (built_tiles_.size() > 0) {
		built_tiles_.erase(built_tiles_.cbegin());
	}

}

// ------------------------------------------------------------------------- //

void BuildManager::createBuilding(bool send_command, int pos_x, int pos_y, int player_id, int build_kind){

	// Check if the current tile is buildable by this player
	int tile_value = basic_map[(pos_x / 16) +	((pos_y / 16) * 38)];
	glm::vec2 build_pos = glm::vec2(pos_x, pos_y);

	if (player_id == 2) {
		if (tile_value == p2_build_tiles[0]) {
			if (NetworkGame::instance().getScene()->map_->checkFourAdjacentTiles(
				build_pos, Tilemap::kTileKind_RoadBuildable) &&
				!checkForBuildings(build_pos)) {

				GameObject* build_object = GameObject::CreateGameObject();
				Sprite* sprite2 = new Sprite(*build_object, "../../../data/images/objects.png", 80, 48, 16, 32);
				build_object->transform_.position_ = glm::vec3(build_pos, 0.0f);
				build_object->addComponent(sprite2);

				/*Building* new_build = new Building();
				new_build->go_ref_ = build_object;
				new_build->build_kind_ = (Building::BuildKind)build_kind;
				new_build->pos_x_ = pos_x;
				new_build->pos_y_ = pos_y;*/
				built_tiles_.push_back(build_pos);


				if (send_command) {
					// Add command to the cmd list for when the net thread starts working again
					BuildData* build_cmd = CreateBuildData(player_id, build_pos);
					NetworkGame::instance().cmd_list_->commands_.push_back(build_cmd);
				}
			}
		}
	}
	else {
		if (tile_value == p1_build_tiles[0]) {
			// Create the object in the position of the mouse if it is allowed
			if (NetworkGame::instance().getScene()->map_->checkFourAdjacentTiles(
					build_pos, Tilemap::kTileKind_RoadBuildable) &&
				!checkForBuildings(build_pos)) {

				GameObject* build_object = GameObject::CreateGameObject();
				Sprite* sprite = new Sprite(*build_object, "../../../data/images/terrain.png", 192, 352, 32, 32);
				build_object->transform_.position_ = glm::vec3(build_pos, 0.0f);
				build_object->addComponent(sprite);

				/*Building* new_build = new Building();
				new_build->go_ref_ = build_object;
				new_build->build_kind_ = (Building::BuildKind)build_kind;
				new_build->pos_x_ = pos_x;
				new_build->pos_y_ = pos_y;*/
				built_tiles_.push_back(build_pos);


				if (send_command) {
					// Add command to the cmd list for when the net thread starts working again
					BuildData* build_cmd = CreateBuildData(player_id, build_pos);
					NetworkGame::instance().cmd_list_->commands_.push_back(build_cmd);
				}
			}
		}
	}

}

bool BuildManager::checkForBuildings(glm::vec2 building_pos){

	for (int i = 0; i < built_tiles_.size(); ++i) {
		if ((int)building_pos.x == (int)built_tiles_[i].x &&
			(int)building_pos.y == (int)built_tiles_[i].y) {
		
			return true;

		}
	}

	return false;
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
