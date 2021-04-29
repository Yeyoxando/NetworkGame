/*
*  Author: Diego Ochando Torres
*  Date: 30/10/2018
*  e-mail: c0022981@my.shu.ac.uk | yeyoxando@gmail.com
*/

#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__ 

#include <vector>
#include <SDL.h>
#include <glm.hpp>

#include "component.h"
#include "component_boxcollider.h"
#include "component_sprite.h"
#include "component_label.h"

//Used as collision tags
enum TypeTags {
  kTypeEmpty = 0,
  kTypeFloor,
  kTypeWall,
  kTypeWorm,
  kTypeBall
};

struct Transform {
  float rotation_;
  glm::vec3 position_;
  glm::vec3 scale_;
  glm::vec2 size_;
};

class GameObject {
public:
//----------------------------- Constructors --------------------------------//

  /**
  * @brief Parameterized GameObject constructor
  * @param tag_id: Corresponding type tag id
  * @param active: Current state, if true, updates and draws himself
  */
  GameObject(const char tag_id, const bool active);

	/**
		* @brief GameObject constructor for database
    * @param id: GameObject id
    * @param tag_id: Corresponding type tag id
    * @param active: Current state, if true, updates and draws himself
    * @param db_load: loading from db
	*/
  GameObject(const int id, const char tag_id, const bool active, bool db_load);

  /**
  * @brief GameObject destructor
  */
  ~GameObject();

//---------------------------------------------------------------------------//

//---------------------------------- Init -----------------------------------//
  /**
  * @brief GameObject empty initializer
  */
  void init();

  /**
  * @brief Parameterized GameObject initializer
  * @param position new gameobject position
  */
  void init(glm::vec3 position);

//---------------------------------------------------------------------------//

//-------------------------------- Factories --------------------------------//
  /**
    * @brief Empty GameObject factory
  */
  static GameObject* CreateGameObject();

	/**
		* @brief GameObject factory for database
    * @param id: GameObject id
    * @param tag_id: Corresponding type tag id
    * @param active: Current state, if true, updates and draws himself
    * @param db_load: loading from db
	*/
	static GameObject* CreateGameObject(const int id, const char tag_id, const bool active, bool db_load);

  /**
    * @brief Empty GameObject with position factory
		* @param position new GameObject position
  */
  static GameObject* CreateGameObject(glm::vec3 position);

  /**
    * @brief Copy GameObject factory
		* @param other GameObject to copy
  */
  static GameObject* CreateGameObject(const GameObject& other);
//---------------------------------------------------------------------------//

//---------------------------- Setters & Getters ----------------------------//
	/**
		* @brief Sets the GameObject name using strcpy to avoid wrong name insertions
	*/
	void set_name(const char* new_name);

	/**
		* @brief Returns the GameObject name
	*/
	char* get_name();

//---------------------------------------------------------------------------//

//---------------------------------- Utils ----------------------------------//
  /**
  * @brief Updates the state of the GameObject and it's components
  */
  virtual void update();

  /**
  * @brief Draws the drawable components on GameObject 
  */
  virtual void draw();

	/**
	* @brief Draws the drawable components on GameObject
	*/
	//virtual void inspector();

  /**
  *  @brief Add a component to this GameObject
  *  @param comp component to add
  */
  void addComponent(Component* comp);

  /**
  *  @brief Copies an existing component to this GameObject
  *  @param comp component to copy
  */
  void copyComponent(Component* comp);

  /**
  * @brief Get a component from this GameObject
  *  @param comp component kind to get
  */
  Component* getComponent(ComponentKind comp);

  /**
  * @brief Removes a component from this GameObject
  *  @param comp component kind to delete
  */
  void removeComponent(ComponentKind comp);

//---------------------------------------------------------------------------//

//------------------------------- Attributes -------------------------------//
  char tag_id_;
  bool active_;
  int id_;
  int components_count_;
  Transform transform_;
  std::vector<Component*> components_;
	static int game_object_count_;
	char name_[32];

//---------------------------------------------------------------------------//
private:
//----------------------------- Constructors --------------------------------//
  /**
   * @brief GameObject constructor
  */
  GameObject();

  /**
  * @brief Parameterized GameObject constructor
  * @param position new game object position
  */
  GameObject(glm::vec3 position);

  /**
  * @brief GameObject copy constructor
  * @param other GameObject to copy
  */
  GameObject(const GameObject&  other);

};

#endif