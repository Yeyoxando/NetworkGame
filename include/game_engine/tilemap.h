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

// Includes

// ------------------------------------------------------------------------- //

class Tilemap {
public:
  Tilemap();
  ~Tilemap();

protected:

};

// ------------------------------------------------------------------------- //

#endif // __TILEMAP_H__