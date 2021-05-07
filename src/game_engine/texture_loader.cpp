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
	sub_sprites_ = std::map <int, SDL_Rect>();

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
