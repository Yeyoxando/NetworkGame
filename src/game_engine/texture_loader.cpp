/*
 *	Author: Diego Ochando Torres
 *  Date: 07/05/2021
 *  e-mail: c0022981@my.shu.ac.uk | yeyoxando@gmail.com
 */

 // ------------------------------------------------------------------------- //

#include "texture_loader.h"
#include "network_game.h"

#include <SDL_image.h>

 // ------------------------------------------------------------------------- //

TextureLoader::TextureLoader() {

	textures_ = std::map <const char*, SDL_Texture*>();
	sub_sprites_ = std::map <int, SubSprite>();

}

// ------------------------------------------------------------------------- //

TextureLoader::~TextureLoader(){

	while (!textures_.empty()) {
		SDL_DestroyTexture(textures_.cbegin()->second);
		textures_.erase(textures_.cbegin());
	}
	textures_.clear();

	sub_sprites_.clear();

}

// ------------------------------------------------------------------------- //

SDL_Texture* TextureLoader::loadTexture(const char* file_name, int* width, int* height){

	// Check if texture was previously loaded to not load it again
	auto it = textures_.cbegin();
	while (it != textures_.cend()) {
		if (!strcmp(file_name, it->first)) {
			//printf("\n[ _warning_ ] Texture [%s] has been loaded earlier.", file_name);
			// Return if previously loaded
			*width = 0; // Fix this, it needs the right size
			*height = 0;
			return it->second;
		}
		++it;
	}

	SDL_Surface* img = IMG_Load(file_name);

	if (img == nullptr) {
		printf("\n[ _error_ ] Error in [%s]: Sprite source (%s) doesn't exist. Check name.", __FUNCTION__, file_name);
		return nullptr;
	}
	else {
		SDL_Texture* texture = SDL_CreateTextureFromSurface(NetworkGame::instance().renderer_, img);
		textures_.insert(std::pair<const char*, SDL_Texture*>(file_name, texture));
		*width = img->w;
		*height = img->h;
		return texture;
	}

}

// ------------------------------------------------------------------------- //

int TextureLoader::loadSubSprite(const char* file_name, SDL_Texture** texture, int pos_x, int pos_y, int width, int height){

	int w, h;
	*texture = loadTexture(file_name, &w, &h);
	SubSprite sub_sprite = { file_name, { pos_x, pos_y, width, height } };

	// Check if sub sprite was previously loaded to not load it again
	auto it = sub_sprites_.cbegin();
	while (it != sub_sprites_.cend()) {
		if (!strcmp(file_name, it->second.tex_name_)) {
			if (it->second.rect_ == sub_sprite.rect_) {
				// Return if previously loaded
				//printf("\n[ _warning_ ] Subsprite [%s] has been loaded earlier.", file_name);
				return it->first;
			}
		}
		++it;
	}

	int id = sub_sprites_.size();
	sub_sprites_.insert(std::pair<int, SubSprite>(id, sub_sprite));
	return id;

}

// ------------------------------------------------------------------------- //
