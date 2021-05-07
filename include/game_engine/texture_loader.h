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

 // ------------------------------------------------------------------------- //

class TextureLoader {
public:
	TextureLoader();
  ~TextureLoader();

	//File route set internally.
	SDL_Texture* loadTexture(const char* file_name, int* width, int* height);

protected:
	std::map <const char*, SDL_Texture*> textures_;
	std::map <int, SDL_Rect> sub_sprites_;

};

// ------------------------------------------------------------------------- //

#endif // __TEXTURE_LOADER_H__
