/*
*  Author: Diego Ochando Torres
*  Date: 27/11/2018
*  e-mail: c0022981@my.shu.ac.uk | yeyoxando@gmail.com
*/

#ifndef __COMPONENT_H__
#define __COMPONENT_H__ 

class GameObject;

enum ComponentKind {
  kComponentKind_Default = 0,
  kComponentKind_Rigidbody,
  kComponentKind_BoxCollider,
  kComponentKind_Sprite,
  kComponentKind_Label,
  kComponentKind_Background,
  kComponentKind_WormScript
};

class Component {
public:
	/**
	*  @brief Component constructor
	*/
  Component();

	/**
	*  @brief Component copy constructor
	*/
  Component(const Component& other);

	/**
	*  @brief Component destructor with virtual inheritance
	*/
  virtual ~Component();

	/**
	*  @brief initializes the component
	*/
  virtual void init();

	/**
	*  @brief input for the component
	*/
  virtual void input();

	/**
	*  @brief update for the component
	*/
  virtual void update();
  
	/**
	*  @brief draw for the component
	*/
	virtual void draw();

	/**
	*  @brief inspector for the component
	*/
  virtual void inspector();

	/**
	*  @brief collision detection for the component
	*  @param other_tag_id id of the other gameobject to compare
	*/
  virtual void onCollision(int other_tag_id);

	/**
	*  @brief collision detection for the component
	*  @param other gameobject to compare
	*/
  virtual void onCollision(GameObject* other);

  int id_;
  static int total_components_;

  GameObject *gameobject_;
  ComponentKind component_tag_;
};

#endif
