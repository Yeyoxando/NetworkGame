/*
*  Author: Diego Ochando Torres
*  Date: 02/11/2018
*  e-mail: c0022981@my.shu.ac.uk | yeyoxando@gmail.com
*/

#ifndef __COMPONENT_BOX_COLLIDER_H__
#define __COMPONENT_BOX_COLLIDER_H__ 

#include <SDL.h>
#include <glm.hpp>

#include "component.h"

class GameObject;

class BoxCollider : public Component {
public:
//----------------------------- Constructors --------------------------------//

  /**
    * @brief BoxCollider default constructor. Return error if its called.
    *        The component needs a reference to the GameObject.
  */
  BoxCollider();

  /**
    * @brief BoxCollider constructor
    * @param gameobject GameObject that the component will belongs
  */
  BoxCollider(GameObject& gameobject);

  /**
    * @brief BoxCollider copy constructor
    * @param other BoxCollider to copy
  */
  BoxCollider(GameObject& gameobject, const BoxCollider& other);

  /**
    * @brief Parameterized BoxCollider constructor
    * @param gameobject GameObject that the component will belongs
    * @param origin initial position of the collider
  */
  BoxCollider(GameObject& game_object, const glm::vec3 origin);

  /**
    * @brief BoxCollider destructor
  */
  ~BoxCollider();

//----------------------------- Initializers --------------------------------//
  /**
    * @brief BoxCollider initializer
  */
  void init();

  /**
    * @brief Parameterized BoxCollider initializer
    * @param origin initial position of the collider
  */
  void init(const glm::vec3 origin);

//------------------------------ Setters ------------------------------------//
      /**
        * @brief color setter
        * @param color color that the BoxCollider will use
      */
  void set_color(const glm::vec4 color);

  /**
    * @brief color setter overload
    * @param r red component
    * @param g green component
    * @param b blue component
  */
  void set_color(const unsigned char r, const unsigned char g,
    const unsigned char b);

//------------------------------ Getters ------------------------------------//
  /**
    * @brief origin getter
    * @result current origin point
  */
  const glm::vec3 origin();

  /**
    * @brief color getter
    * @result current collider color
  */
  const glm::vec4 color();

//---------------------------------- Methods --------------------------------//
	/**
		* @brief check if exist collision with another collider
		* @param other_collider collider to check
		* @result if the objects are colliding returns true
	*/
	bool checkCollision(const BoxCollider& other_collider);

  /**
    * @brief check if exist collision with both colliders (Static to use it without having a collider
    * @param collider1 first collider to check
		* @param collider2 second collider to check
    * @result if the objects are colliding returns true
  */
  static bool checkCollision(const BoxCollider& collider1,
                             const BoxCollider& collider2);

  /**
    * @brief check if exist collision with a point
    * @param point point to check
    * @result if the point and collider are colliding returns true
  */
  static bool checkCollision(const BoxCollider& collider,
                             const glm::vec2 point);

  /**
    * @brief updates the collider attributes with GameObject transform  (Default component call)
  */
  void update() override;

  /**
    * @brief draws the BoxCollider (Default component call)
  */
  void draw() override;

	/**
		* @brief inspector for BoxCollider
	*/
  //void inspector();

  /**
    * @brief draws the BoxCollider with a diagonal line (Debug method)
  */
  void drawWithDiagonal();

  glm::vec3 origin_;
  glm::vec4 color_;

protected:
//------------------------------ Atributtes ---------------------------------//
  glm::vec3 end_;
  SDL_Rect rect_;

};

#endif
