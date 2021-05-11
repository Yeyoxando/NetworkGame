/*
 *	Author: Diego Ochando Torres
 *  Date: 28/03/2021
 *  e-mail: c0022981@my.shu.ac.uk | yeyoxando@gmail.com
 */

 // ------------------------------------------------------------------------- //

#include "tilemap.h"
#include "network_game.h"
#include "network_game_maps.h"

 // ------------------------------------------------------------------------- //

Tilemap::Tilemap(){

	sub_sprite_refs_ = std::vector<SubSprite>(0);
	draw_grid_ = false;

}

// ------------------------------------------------------------------------- //

Tilemap::~Tilemap(){



}

// ------------------------------------------------------------------------- //

void Tilemap::loadSubSprites(){

	// 0: grass player 1
	sub_sprite_refs_.push_back({ "../../../data/images/terrain.png", {48, 0, 16, 16 } });
	// 1: grass player 2
	sub_sprite_refs_.push_back({ "../../../data/images/terrain.png", {48, 0, 16, 16 } });
	// 2: trees
	sub_sprite_refs_.push_back({ "../../../data/images/terrain.png", {224, 192, 16, 16 } });
	// 3:P1 castle 1
	sub_sprite_refs_.push_back({ "../../../data/images/terrain.png", {160, 352, 16, 16 } });
	// 4:P1 castle 2
	sub_sprite_refs_.push_back({ "../../../data/images/terrain.png", {176, 352, 16, 16 } });
	// 5:P1 castle 3
	sub_sprite_refs_.push_back({ "../../../data/images/terrain.png", {160, 368, 16, 16 } });
	// 6:P1 castle 4
	sub_sprite_refs_.push_back({ "../../../data/images/terrain.png", {176, 368, 16, 16 } });
	// 7:P2 castle 1
	sub_sprite_refs_.push_back({ "../../../data/images/objects.png", {96, 80, 16, 16 } });
	// 8:P2 castle 2
	sub_sprite_refs_.push_back({ "../../../data/images/objects.png", {112, 80, 16, 16 } });
	// 9:P2 castle 3
	sub_sprite_refs_.push_back({ "../../../data/images/objects.png", {96, 96, 16, 16 } });
	// 10:P2 castle 4
	sub_sprite_refs_.push_back({ "../../../data/images/objects.png", {112, 96, 16, 16 } });
	// 11: water
	sub_sprite_refs_.push_back({ "../../../data/images/terrain.png", {0, 16, 16, 16 } });
	// 12: bridge 1
	sub_sprite_refs_.push_back({ "../../../data/images/objects.png", {0, 16, 16, 16 } });
	// 13: bridge 2
	sub_sprite_refs_.push_back({ "../../../data/images/objects.png", {16, 16, 16, 16 } });
	// 14: bridge 3
	sub_sprite_refs_.push_back({ "../../../data/images/objects.png", {32, 16, 16, 16 } });
	// 15: water_ledge right
	sub_sprite_refs_.push_back({ "../../../data/images/terrain.png", {32, 32, 16, 16 } });
	// 16: water_ledge left
	sub_sprite_refs_.push_back({ "../../../data/images/terrain.png", {64, 32, 16, 16 } });
	// 17: water_ledge top
	sub_sprite_refs_.push_back({ "../../../data/images/terrain.png", {48, 48, 16, 16 } });
	// 18: water_ledge bottom
	sub_sprite_refs_.push_back({ "../../../data/images/terrain.png", {48, 16, 16, 16 } });
	// 19: water_ledge top_right
	sub_sprite_refs_.push_back({ "../../../data/images/terrain.png", {80, 64, 16, 16 } });
	// 20: water_ledge top_left
	sub_sprite_refs_.push_back({ "../../../data/images/terrain.png", {64, 64, 16, 16 } });
	// 21: water_ledge bottom_right
	sub_sprite_refs_.push_back({ "../../../data/images/terrain.png", {112, 64, 16, 16 } });
	// 22: water_ledge bottom_left
	sub_sprite_refs_.push_back({ "../../../data/images/terrain.png", {96, 64, 16, 16 } });
	// 23: water_ledge top_right corner
	sub_sprite_refs_.push_back({ "../../../data/images/terrain.png", {32, 48, 16, 16 } });
	// 24: water_ledge top_left corner
	sub_sprite_refs_.push_back({ "../../../data/images/terrain.png", {64, 48, 16, 16 } });
	// 25: water_ledge bottom_right corner
	sub_sprite_refs_.push_back({ "../../../data/images/terrain.png", {32, 16, 16, 16 } });
	// 26: water_ledge bottom_left corner
	sub_sprite_refs_.push_back({ "../../../data/images/terrain.png", {64, 16, 16, 16 } });
	// 27: water_river vertical
	sub_sprite_refs_.push_back({ "../../../data/images/terrain.png", {0, 144, 16, 16 } });
	// 28: water_river top entrance
	sub_sprite_refs_.push_back({ "../../../data/images/terrain.png", {0, 384, 16, 16 } });
	// 29: water_river top_right turn
	sub_sprite_refs_.push_back({ "../../../data/images/terrain.png", {0, 224, 16, 16 } });
	// 30: water_river top_left turn
	sub_sprite_refs_.push_back({ "../../../data/images/terrain.png", {16, 224, 16, 16 } });
	// 31: water_river bottom_right turn
	sub_sprite_refs_.push_back({ "../../../data/images/terrain.png", {0, 304, 16, 16 } });
	// 32: water_river bottom_left turn
	sub_sprite_refs_.push_back({ "../../../data/images/terrain.png", {16, 304, 16, 16 } });
	// 33: water_river bottom entrance
	sub_sprite_refs_.push_back({ "../../../data/images/terrain.png", {16, 384, 16, 16 } });
	// 34: pathway horizontal
	sub_sprite_refs_.push_back({ "../../../data/images/terrain.png", {48, 80, 16, 16 } });
	// 35: pathway vertical
	sub_sprite_refs_.push_back({ "../../../data/images/terrain.png", {16, 96, 16, 16 } });
	// 36: pathway top right
	sub_sprite_refs_.push_back({ "../../../data/images/terrain.png", {48, 112, 16, 16 } });
	// 37: pathway top left
	sub_sprite_refs_.push_back({ "../../../data/images/terrain.png", {64, 112, 16, 16 } });
	// 38: pathway bottom right
	sub_sprite_refs_.push_back({ "../../../data/images/terrain.png", {48, 96, 16, 16 } });
	// 39: pathway bottom left
	sub_sprite_refs_.push_back({ "../../../data/images/terrain.png", {64, 96, 16, 16 } });
	// 40: P1 tent barracks
	sub_sprite_refs_.push_back({ "../../../data/images/objects.png", {64, 64, 16, 16 } });
	// 41: P2 tent barracks
	sub_sprite_refs_.push_back({ "../../../data/images/objects.png", {48, 64, 16, 16 } });
	// 42: P1 tent barracks rotated
	sub_sprite_refs_.push_back({ "../../../data/images/objects.png", {96, 32, 16, 16 } });
	// 43: P2 tent barracks rotated
	sub_sprite_refs_.push_back({ "../../../data/images/objects.png", {80, 32, 16, 16 } });


	for (int i = 0; i < sub_sprite_refs_.size(); ++i) {
		SubSprite sref = sub_sprite_refs_[i];
		SDL_Texture* tex;
		NetworkGame::instance().tex_loader_->loadSubSprite(sref.tex_name_, &tex,
			sref.rect_.x, sref.rect_.y, sref.rect_.w, sref.rect_.h);
	}

}

// ------------------------------------------------------------------------- //

int Tilemap::checkFourAdjacentTiles(glm::vec2 tile_pos, TileKind tile_kind){

	return 0;

}

// ------------------------------------------------------------------------- //

int Tilemap::checkEightAdjacentTiles(glm::vec2 tile_pos, TileKind tile_kind){

	return 0;

}

// ------------------------------------------------------------------------- //

void Tilemap::draw(){

	// Base map
	for (int y = 0; y < 25; ++y) {
		for (int x = 0; x < 38; ++x) {
			int tile_value = basic_map[x + y * 38];

			SDL_Rect rect_instance = { 0, 0, 0, 0 };
			rect_instance.x = x * 16;
			rect_instance.y = y * 16;
			rect_instance.w = 16;
			rect_instance.h = 16;

			float rotation_degrees = 0.0f;
			SDL_Point final_pivot = { 0, 0 };

			int w, h;
			SDL_Texture* texture = NetworkGame::instance().tex_loader_->loadTexture(
				sub_sprite_refs_[tile_value].tex_name_, &w, &h);

			SDL_RenderCopyEx(NetworkGame::instance().renderer_, texture,
				&sub_sprite_refs_[tile_value].rect_,
				&rect_instance, rotation_degrees, &final_pivot, SDL_FLIP_NONE);
		}
	}

	// Detail map
	for (int y = 0; y < 25; ++y) {
		for (int x = 0; x < 38; ++x) {
			int tile_value = detail_map[x + y * 38];
			if (tile_value == 0) continue;

			SDL_Rect rect_instance = { 0, 0, 0, 0 };
			rect_instance.x = x * 16;
			rect_instance.y = y * 16;
			rect_instance.w = 16;
			rect_instance.h = 16;

			float rotation_degrees = 0.0f;
			SDL_Point final_pivot = { 0, 0 };

			int w, h;
			SDL_Texture* texture = NetworkGame::instance().tex_loader_->loadTexture(
				sub_sprite_refs_[tile_value].tex_name_, &w, &h);

			SDL_RenderCopyEx(NetworkGame::instance().renderer_, texture,
				&sub_sprite_refs_[tile_value].rect_,
				&rect_instance, rotation_degrees, &final_pivot, SDL_FLIP_NONE);
		}
	}

	// Grid
	if (draw_grid_) {
		SDL_SetRenderDrawColor(NetworkGame::instance().renderer_, 255, 0, 0, 255);
		for (int y = 0; y < 24; ++y) {
			for (int x = 0; x < 37; ++x) {
				SDL_RenderDrawLine(NetworkGame::instance().renderer_, 16.0f + 16.0f * x,
					0.0f, 16.0f + 16.0f * x, 400.0f);
				SDL_RenderDrawLine(NetworkGame::instance().renderer_, 0.0f,
					16.0f + 16.0f * y, 608.0f, 16.0f + 16.0f * y);
			}
		}
	}

}

// ------------------------------------------------------------------------- //
