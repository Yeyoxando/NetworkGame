/*
*  Author: Diego Ochando Torres
*  Date: 30-10-2018
*  e-mail: c0022981@my.shu.ac.uk | yeyoxando@gmail.com
*/

#include "game_object.h"
#include "network_game.h"
//#include "editor.h"

/*struct ExampleAppLog {
	void    AddLog(const char* fmt, ...) IM_FMTARGS(2);
};*/

int GameObject::game_object_count_ = 0;

//----------------------------- Constructor ---------------------------------//
GameObject::GameObject() {
	memset(name_, '\0', sizeof(name_));
	set_name("Empty Object");
  id_ = game_object_count_;
  ++game_object_count_;
  tag_id_ = kTypeEmpty;
  active_ = true;
  components_count_ = 0;
}

GameObject::GameObject(glm::vec3 position) {
	memset(name_, '\0', sizeof(name_));
	set_name("Empty Object");
  id_ = game_object_count_;
  ++game_object_count_;
  tag_id_ = kTypeEmpty;
  active_ = true;
  components_count_ = 0;
  transform_.position_ = position;
}

GameObject::GameObject(const char tag_id, const bool active) {
	memset(name_, '\0', sizeof(name_));
	set_name("Empty Object");
  id_ = game_object_count_;
  ++game_object_count_;
  tag_id_ = tag_id;
  active_ = active;
  components_count_ = 0;
  transform_.position_ = glm::vec3(0, 0, 0);
  transform_.scale_ = glm::vec3(1, 1, 1);
  transform_.rotation_ = 0.0f;
  transform_.size_ = glm::vec2(1, 1);
}

GameObject::GameObject(const int id, const char tag_id, const bool active, bool db_load) {
	memset(name_, '\0', sizeof(name_));
	set_name("Empty Object");
  id_ = game_object_count_;
  ++game_object_count_;
  tag_id_ = tag_id;
  active_ = active;
  components_count_ = 0;
	transform_.position_ = glm::vec3(0, 0, 0);
	transform_.scale_ = glm::vec3(1, 1, 1);
	transform_.rotation_ = 0.0f;
	transform_.size_ = glm::vec2(1, 1);
}

//---------------------------------------------------------------------------//

//-------------------------- Copy Constructor -------------------------------//

GameObject::GameObject(const GameObject& other) {
	memset(name_, '\0', sizeof(name_));
	char cpy_name[32];
	memset(cpy_name, '\0', 32);
	strcpy(cpy_name, other.name_);
	if (cpy_name[28] == '\0') {
		strcat(cpy_name, "_copy");
	}
	set_name(cpy_name);
  id_ = game_object_count_;
  ++game_object_count_;
  tag_id_ = other.tag_id_;
  active_ = other.active_;
  components_count_ = 0;
  for (int i = 0; i < other.components_count_; ++i) {
    copyComponent(other.components_[i]);
  }
  transform_.position_ = other.transform_.position_;
  transform_.scale_ = other.transform_.scale_;
  transform_.rotation_ = other.transform_.rotation_;
  transform_.size_ = other.transform_.size_;
}

//---------------------------------------------------------------------------//

//----------------------------- Destructor ----------------------------------//
GameObject::~GameObject() {
  while (components_.size() > 0) {
		delete components_[components_.size()-1];
    components_.pop_back();
  }
  --GameObject::game_object_count_;
}

//---------------------------------------------------------------------------//

//----------------------------- Initializer ---------------------------------//
void GameObject::init() {
  tag_id_ = kTypeEmpty;
  active_ = true;
	transform_.position_ = glm::vec3(0, 0, 0);
	transform_.scale_ = glm::vec3(1, 1, 1);
	transform_.rotation_ = 0.0f;
	transform_.size_ = glm::vec2(1, 1);
}

void GameObject::init(glm::vec3 position) {
  tag_id_ = kTypeEmpty;
  active_ = true;
	transform_.position_ = position;
	transform_.scale_ = glm::vec3(1, 1, 1);
	transform_.rotation_ = 0.0f;
	transform_.size_ = glm::vec2(1, 1);
}

//---------------------------------------------------------------------------//

//-------------------------------- Factory ----------------------------------//
GameObject* GameObject::CreateGameObject() {
  if (game_object_count_ < MAX_GAME_OBJECTS) {
    GameObject* go = new GameObject();
    go->init();
		NetworkGame::instance().scene_->addGameObject(go);
    return go;
  }
  else {
    printf("\n[ _error_ ] Error in [%s]: GameObject limit reached (%d).", __FUNCTION__, MAX_GAME_OBJECTS);
    return nullptr;
  }
}

GameObject* GameObject::CreateGameObject(const int id, const char tag_id, const bool active, bool db_load) {
	if (game_object_count_ < MAX_GAME_OBJECTS) {
		GameObject* go = new GameObject(id, tag_id, active, db_load);
    NetworkGame::instance().scene_->addGameObject(go);
		return go;
	}
	else {
    printf("\n[ _error_ ] Error in [%s]: GameObject limit reached (%d).", __FUNCTION__, MAX_GAME_OBJECTS);
		return nullptr;
	}
}

GameObject* GameObject::CreateGameObject(glm::vec3 position) {
  if (game_object_count_ < MAX_GAME_OBJECTS) {
    GameObject* go = new GameObject(position);
    go->init(position);
    NetworkGame::instance().scene_->addGameObject(go);
    return go;
  }
  else {
    printf("\n[ _error_ ] Error in [%s]: GameObject limit reached (%d).", __FUNCTION__, MAX_GAME_OBJECTS);
    return nullptr;
  }
}

GameObject* GameObject::CreateGameObject(const GameObject& other) {
  if (game_object_count_ < MAX_GAME_OBJECTS) {
    GameObject* go = new GameObject(other);
    NetworkGame::instance().scene_->addGameObject(go);
    return go;
  }
  else {
    printf("\n[ _error_ ] Error in [%s]: GameObject limit reached (%d).", __FUNCTION__, MAX_GAME_OBJECTS);
    return nullptr;
  }
}

//---------------------------------------------------------------------------//

//---------------------------- Setters & Getters ----------------------------//
void GameObject::set_name(const char* new_name) {
	memset(name_, '\0', sizeof(name_));
	strcpy(name_, new_name);
}

char* GameObject::get_name() {
	return (char*)name_;
}

//---------------------------------------------------------------------------//

//-------------------------------- Update -----------------------------------//
void GameObject::update() {
  if (active_) {
    for (int i = 0; i < components_count_; ++i) {
      components_[i]->update();
    }
  }
}
//---------------------------------------------------------------------------//

//--------------------------------- Draw ------------------------------------//
void GameObject::draw() {
  if (active_) {
    for (int i = 0; i < components_count_; ++i) {
      components_[i]->draw();
    }
  }
}

//---------------------------------------------------------------------------//

//------------------------------ Inspector ----------------------------------//
/*void GameObject::inspector() {

	if (ImGui::TreeNode("GameObject")) {
		static char buf[32];
		//memcpy(buf, tag_id_, strlen(tag_id_));
		ImGui::InputText("ID Tag", buf, IM_ARRAYSIZE(buf));
		
		//This line will be the last one
		ImGui::TreePop();
	}

	for (int i = 0; i < components_count_; ++i) {
		components_[i]->inspector();
	}
}*/

//---------------------------------------------------------------------------//

void GameObject::addComponent(Component* comp) {
  if (nullptr != comp) {
    bool existing_component = false;
    for (int i = 0; i < components_count_; ++i) {
      if (components_[i]->component_tag_ == comp->component_tag_) {
        existing_component = true;
      }
    }

    if (!existing_component) {
      components_.push_back(comp);
      ++components_count_;
    }
    else {
			printf("\n[ _error_ ] Error in [%s]: GameObject already contains this component.", __FUNCTION__);
    }
  }
  else {
    printf("\n[ _error_ ] Error in [%s]: Component null or not valid.", __FUNCTION__);
  }
}

void GameObject::copyComponent(Component* comp) {

  switch (comp->component_tag_) {
    case kComponentKind_Label: {
      Label* current_lbl = (Label*)comp;
      Label* new_lbl = new Label(*this, *current_lbl);
      addComponent(new_lbl);
      break;
    }
    case kComponentKind_BoxCollider: {
      BoxCollider* current_box = (BoxCollider*)comp;
      BoxCollider* new_box = new BoxCollider(*this, *current_box);
      addComponent(new_box);
      break;
    }
    case kComponentKind_Sprite: {
      Sprite* current_spr = (Sprite*)comp;
      Sprite* new_spr = new Sprite(*this, *current_spr);
      addComponent(new_spr);
      break;
    }
    default: {
      break;
    }
  }

}

Component* GameObject::getComponent(ComponentKind comp) {
  for (int i = 0; i < components_count_; ++i) {
    if (components_[i]->component_tag_ == comp) {
      return components_[i];
    }
  }
  printf("\n[ _error_ ] Error in [%s]: GameObject doesn't have any component of this kind.", __FUNCTION__);
  return nullptr;
}

void GameObject::removeComponent(ComponentKind comp) {
  Component* component = nullptr;
  int position = 0;
  for (int i = 0; i < components_count_; ++i) {
    if (components_[i]->component_tag_ == comp) {
      component = components_[i];
      position = i;
      break;
    }
  }
  if (nullptr != component) {
    components_.erase(components_.begin() + position);
    --components_count_;
  }
  else {
    printf("\n[ _error_ ] Error in [%s]: GameObject doesn't have any component of this kind.", __FUNCTION__);
  }
}