/*
 *	Author: Diego Ochando Torres
 *  Date: 28/03/2021
 *  e-mail: c0022981@my.shu.ac.uk | yeyoxando@gmail.com
 */

#ifndef __TILEMAP_H__
#define __TILEMAP_H__

// ------------------------------------------------------------------------- //
// Extra note AI: Make waypoints along the map and calculate only a little A* path to get to one of them, if the character collides with something recalculate
// from 0 to 5 or something are walkable tiles, anything else is not, this will be used for 
// the collision map, involving AI, building mechanics and everything else

#include <vector>
#include <glm.hpp>
#include "texture_loader.h"

// ------------------------------------------------------------------------- //

class Tilemap {
public:
  Tilemap();
  ~Tilemap();

  enum TileKind {
    kTileKind_Blocked = 0,
    kTileKind_RoadBuildable = 1,
    kTileKind_FieldBuildable = 2,
    kTileKind_Tree = 3,
    kTileKind_Rock = 4,
    kTileKind_FarmGrass = 5,
    kTileKind_Tower = 6,
    kTileKind_House = 7,
    kTileKind_Walkable = 8,
  };

  void loadSubSprites();

  int checkFourAdjacentTiles(glm::vec2 tile_pos, TileKind tile_kind);
  int checkEightAdjacentTiles(glm::vec2 tile_pos, TileKind tile_kind);

  //get buildable tiles(player id, field/road) (used when building mode)

  //void loadMap(const int map[950]);
  void draw();

  bool draw_grid_;

protected:
  int static_map_[950];
  int dinamyc_map_[950];
  std::vector<SubSprite> sub_sprite_refs_;
  //std::vector<SDL_Texture*> textures_;


};

// ------------------------------------------------------------------------- //

#endif // __TILEMAP_H__