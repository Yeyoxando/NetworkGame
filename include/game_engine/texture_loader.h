/*
 *	Author: Diego Ochando Torres
 *  Date: 07/05/2021
 *  e-mail: c0022981@my.shu.ac.uk | yeyoxando@gmail.com
 */

#ifndef __TEXTURE_LOADER_H__
#define __TEXTURE_LOADER_H__

// ------------------------------------------------------------------------- //
/*
	Load all textures here
	Reference them with an ID on the sprite and a SDL_Rect to subdivide the sprite if needed
*/

#include <map>
#include <SDL.h>
#include <glm.hpp>

 // ------------------------------------------------------------------------- //
struct SubSprite {
	const char* tex_name_;
	SDL_Rect rect_;
};

class TextureLoader {
public:
	TextureLoader();
  ~TextureLoader();

	//File route set internally.
	SDL_Texture* loadTexture(const char* file_name, int* width, int* height);
	
	int loadSubSprite(const char* file_name, SDL_Texture** texture, int pos_x, int pos_y, int width, int height);

protected:
	std::map <const char*, SDL_Texture*> textures_;
	std::map <const char*, glm::vec2> texture_sizes_;
	std::map <int, SubSprite> sub_sprites_;

	friend class Sprite;

};

// ------------------------------------------------------------------------- //

#endif // __TEXTURE_LOADER_H__
