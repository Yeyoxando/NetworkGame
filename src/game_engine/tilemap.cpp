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

}

// ------------------------------------------------------------------------- //

Tilemap::~Tilemap(){



}

// ------------------------------------------------------------------------- //

void Tilemap::loadSubSprites(){

	// 0: grass
	sub_sprite_refs_.push_back({ "../../../data/images/terrain.png", {48, 0, 16, 16 } });
	// 1: grass w/ flowers
	sub_sprite_refs_.push_back({ "../../../data/images/terrain.png", {96, 0, 16, 16 } });

	for (int i = 0; i < sub_sprite_refs_.size(); ++i) {
		SubSprite sref = sub_sprite_refs_[i];
		SDL_Texture* tex;
		NetworkGame::instance().tex_loader_->loadSubSprite(sref.tex_name_, &tex,
			sref.rect_.x, sref.rect_.y, sref.rect_.w, sref.rect_.h);
	}

}

// ------------------------------------------------------------------------- //

void Tilemap::draw(){

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

}

// ------------------------------------------------------------------------- //
