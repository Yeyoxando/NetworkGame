/*
 *	Author: Diego Ochando Torres
 *  Date: 21/11/2018
 *  e-mail: c0022981@my.shu.ac.uk | yeyoxando@gmail.com
 */

#include "scene.h"

//----------------------------- Constructors --------------------------------//

Scene::Scene() {

	game_objects_ = std::vector<GameObject*>();
  
}

Scene::Scene(const Scene& other) {

	game_objects_ = other.game_objects_;

}

Scene::~Scene() {
  for (auto &go : game_objects_) {
    delete go;
  }
}

//---------------------------------------------------------------------------//

//----------------------------- Initializers --------------------------------//

void Scene::init() {
  
  map_ = new Tilemap();
  map_->loadSubSprites();

}

//---------------------------------------------------------------------------//

//-------------------------------- Methods ----------------------------------//

void Scene::addGameObject(GameObject *game_object) {

  if (nullptr != game_object) {
    game_objects_.push_back(game_object);
  }

}

void Scene::deleteGameObject(unsigned int index) {
  if (0 <= index && game_objects_.size() > index) {
    delete game_objects_[index];
    game_objects_.erase(game_objects_.begin() + index);
    for (int i = index; i < game_objects_.size(); ++i) {
      --game_objects_[i]->id_;
    }
  }
}

void Scene::deleteGameObject(GameObject *game_object) {
  int index = 0;
  for (auto &go : game_objects_) {
    if (go->id_ == game_object->id_) {
      deleteGameObject(index);
      break;
    }
    index++;
  }
}

void Scene::deleteLastGameObject() {
  delete game_objects_[game_objects_.size() - 1];
  game_objects_.pop_back();
}

void Scene::input() {

}

void Scene::update() {
	for (int i = 0; i < game_objects_.size(); ++i) {
    game_objects_[i]->update();
  }

  //Test: working on collision with tags
  //Notes:
  //       A reference to the scene could be used in the game object to get all the other objects
  //       The check collision method could be modifies to accept only the other collider or implemented an overload with only one parameter
  //       The check fails sometimes because the chipmunk collision happens before the boxcollider collision, solution: make slightly bigger boxcolliders
 /* unsigned int i = 1;
  BoxCollider *collider_1 = nullptr;
  BoxCollider *collider_2 = nullptr;
  for (auto &go : game_objects_) {
    collider_1 = (BoxCollider*)go->getComponent(kComponentKind_BoxCollider);
    if (nullptr != collider_1) {
      for (unsigned int j = i; j < game_objects_.size(); j++) {
        collider_2 = (BoxCollider*)game_objects_.at(j)->getComponent(kComponentKind_BoxCollider);
        if (nullptr != collider_2
          ) {
          if (collider_1->checkCollision(*collider_1, *collider_2)) {
            for (auto &component : go->components_) {
              component->onCollision(game_objects_.at(j)->tag_id_);
              component->onCollision(game_objects_.at(j));
            }
            for (auto &component : game_objects_.at(j)->components_) {
              component->onCollision(go->tag_id_);
              component->onCollision(go);
            }
          }
        }
      }
    }
    ++i;
  }*/

}

void Scene::draw() {
  map_->draw();

  for (auto &go : game_objects_) {
    go->draw();
  }
}

void Scene::finish() {
  while (game_objects_.size() > 0) {
    deleteLastGameObject();
  }

  delete map_;
}

//---------------------------------------------------------------------------//
