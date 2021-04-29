/*
 *	Author: Diego Ochando Torres
 *  Date: 21/11/2018
 *  e-mail: c0022981@my.shu.ac.uk | yeyoxando@gmail.com
 */

#ifndef __SCENE_H__
#define __SCENE_H__ 

#include <vector>

#include "game_object.h"

#define MAX_GAME_OBJECTS 256

class Scene {
public:
  //----------------------------- Constructors --------------------------------//
  Scene();
  Scene(const Scene& other);
  ~Scene();

  //----------------------------- Initializers --------------------------------//
  void init();

  //-------------------------------- Methods ----------------------------------//
  /**
   * @brief Inserts a GameObject into game_objects_ vector
   * @params game_object The GameObject to insert
  */
  void addGameObject(GameObject *game_object);

  /**
   * @brief Deletes a GameObject from game_objects_ vector
   * @params index The index of the GameObject
  */
  void deleteGameObject(unsigned int index);

  /**
   * @brief Deletes a GameObject from game_objects_ vector
   * @params game_object The GameObject to delete
  */
  void deleteGameObject(GameObject *game_object);

  /**
   * @brief Deletes the last GameObject from game_objects_ vector
  */
  void deleteLastGameObject();

/**
 * @brief Cleans all the data on the Scene
*/
  void finish();

  void input();
  void update();
  void draw();

  //------------------------------ Attributes ---------------------------------//

	std::vector<GameObject*> game_objects_;
	int id_;

};

#endif